#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <string>

#include "common/quick_utils.h"
#include "internal/CHcaCipher.h"
#include "internal/CHcaData.h"
#include "kawashima/hca/CHcaCipherConverter.h"
#include "kawashima/hca/hca_native.h"
#include "kawashima/hca/hca_utils.h"
#include "takamori/exceptions/CArgumentException.h"
#include "takamori/exceptions/CException.h"
#include "takamori/exceptions/CFormatException.h"

#ifdef _MSC_VER
#undef max
#undef min
#endif

CGSS_NS_BEGIN

#define ENSURE_READ_ALL_BUFFER(buffer, size)                                   \
    bufferSize = size;                                                         \
    actualRead = stream->Read(buffer, bufferSize, 0, bufferSize);              \
    do {                                                                       \
        if (actualRead < bufferSize) {                                         \
            throw CException(CGSS_OP_FORMAT_ERROR, "Unexpected end of file."); \
        }                                                                      \
    } while (0)

CHcaCipherConverter::CHcaCipherConverter(
    IStream *stream, const HCA_CIPHER_CONFIG &cryptFrom, const HCA_CIPHER_CONFIG &cryptTo
)
    : MyBase(stream), _cipherFrom(), _cipherTo() {
    _headerBuffer = nullptr;
    clone(cryptFrom, _ccFrom);
    clone(cryptTo, _ccTo);
    _position = 0;
    InitializeExtra();
}

CHcaCipherConverter::~CHcaCipherConverter() {
    if (_cipherFrom) {
        delete _cipherFrom;
        _cipherFrom = nullptr;
    }
    if (_cipherTo) {
        delete _cipherTo;
        _cipherTo = nullptr;
    }
    if (_headerBuffer) {
        delete[] _headerBuffer;
        _headerBuffer = nullptr;
    }
    for (const auto &ptr : _blockBuffers) {
        delete[] ptr.second;
    }
    _blockBuffers.clear();
}

void CHcaCipherConverter::InitializeExtra() {
    const auto &hcaInfo = _hcaInfo;
    auto &ccFrom        = _ccFrom;
    const auto &ccTo    = _ccTo;
    ccFrom.cipherType   = hcaInfo.cipherType;
    _cipherFrom         = new CHcaCipher(ccFrom);
    _cipherTo           = new CHcaCipher(ccTo);
}

auto CHcaCipherConverter::ConvertHeader() -> const std::uint8_t * {
    if (_headerBuffer) {
        return _headerBuffer;
    }
    const auto &hcaInfo = _hcaInfo;
    const auto stream   = _baseStream;
    std::uint32_t bufferSize;
    std::uint32_t actualRead;

    auto *headerBuffer = new std::uint8_t[hcaInfo.dataOffset];
    _headerBuffer      = headerBuffer;
    stream->Seek(0, StreamSeekOrigin::Begin);
    ENSURE_READ_ALL_BUFFER(headerBuffer, hcaInfo.dataOffset);

    std::uint32_t cursor = 0;

    // HCA
    cursor += sizeof(HCA_FILE_HEADER);

    // FMT
    cursor += sizeof(HCA_FORMAT_HEADER);

    // COMP or DEC
    if (areMagicMatch(
            reinterpret_cast<HCA_COMPRESS_HEADER *>(headerBuffer + cursor)->comp, Magic::COMPRESS
        )) {
        cursor += sizeof(HCA_COMPRESS_HEADER);
    } else if (areMagicMatch(
                   reinterpret_cast<HCA_DECODE_HEADER *>(headerBuffer + cursor)->dec, Magic::DECODE
               )) {
        cursor += sizeof(HCA_DECODE_HEADER);
    }

    // VBR
    const auto *vbr = reinterpret_cast<HCA_VBR_HEADER *>(headerBuffer + cursor);
    if (areMagicMatch(vbr->vbr, Magic::VBR)) {
        cursor += sizeof(HCA_VBR_HEADER);
    }

    // ATH
    const auto *ath = reinterpret_cast<HCA_ATH_HEADER *>(headerBuffer + cursor);
    if (areMagicMatch(ath->ath, Magic::ATH)) {
        cursor += sizeof(HCA_ATH_HEADER);
    }

    // LOOP
    const auto *loop = reinterpret_cast<HCA_LOOP_HEADER *>(headerBuffer + cursor);
    if (areMagicMatch(loop->loop, Magic::LOOP)) {
        cursor += sizeof(HCA_LOOP_HEADER);
    }

    // CIPH
    auto *ciph = reinterpret_cast<HCA_CIPHER_HEADER *>(headerBuffer + cursor);
    if (areMagicMatch(ciph->ciph, Magic::CIPHER)) {
        std::uint16_t newCipherType = _ccTo.cipherType;
        newCipherType               = bswap(newCipherType);
        ciph->type                  = newCipherType;
    }

    // Recompute checksum and write to the header.
    const auto newHeaderChecksum = ComputeChecksum(headerBuffer, hcaInfo.dataOffset - 2, 0);
    *(std::uint16_t *)(headerBuffer + hcaInfo.dataOffset - 2) = bswap(newHeaderChecksum);
    return headerBuffer;
}

auto CHcaCipherConverter::ConvertBlock(std::uint32_t blockIndex) -> const std::uint8_t * {
    auto &blockBuffers = _blockBuffers;
    {
        const auto item = blockBuffers.find(blockIndex);
        if (item != blockBuffers.cend()) {
            return item->second;
        }
    }

    const auto &hcaInfo    = _hcaInfo;
    const auto stream      = _baseStream;
    const auto *cipherFrom = _cipherFrom, *cipherTo = _cipherTo;
    std::uint32_t bufferSize;
    std::uint32_t actualRead;

    stream->Seek(hcaInfo.dataOffset + blockIndex * hcaInfo.blockSize, StreamSeekOrigin::Begin);
    auto blockBuffer         = new std::uint8_t[hcaInfo.blockSize];
    blockBuffers[blockIndex] = blockBuffer;
    ENSURE_READ_ALL_BUFFER(blockBuffer, hcaInfo.blockSize);

    if (ComputeChecksum(blockBuffer, hcaInfo.blockSize, 0) != 0) {
        char numberBuffer[20] = {0};
        sprintf(numberBuffer, "%" PRIu32, blockIndex);
        throw CFormatException(
            std::string("CHcaCipherConverter::ConvertBlock @ Block#") + numberBuffer
        );
    }

    // Decipher.
    const auto validDataSize = static_cast<std::uint32_t>(hcaInfo.blockSize - 2);
    cipherFrom->Decrypt(blockBuffer, validDataSize);

    // Check magic piece of plain text.
    CHcaData data(blockBuffer, hcaInfo.blockSize, hcaInfo.blockSize);
    const auto magic = data.GetBit(16);
    if (magic != 0xffff) {
        char numberBuffer[20] = {0};
        sprintf(numberBuffer, "%" PRIu32, blockIndex);
        throw CFormatException(
            std::string("CHcaCipherConverter::ConvertBlock @ Block#") + numberBuffer
        );
    }

    // Recipher.
    cipherTo->Encrypt(blockBuffer, validDataSize);

    // Fix block checksum.
    const auto checksum = ComputeChecksum(blockBuffer, validDataSize, 0);
    *(std::uint16_t *)(blockBuffer + validDataSize) = bswap(checksum);
    return blockBuffer;
}

auto CHcaCipherConverter::GetLength() -> std::uint64_t {
    const auto &hcaInfo = _hcaInfo;
    return hcaInfo.dataOffset + hcaInfo.blockCount * hcaInfo.blockSize;
}

void CHcaCipherConverter::SetPosition(std::uint64_t value) {
    _position = value;
}

auto CHcaCipherConverter::GetPosition() -> std::uint64_t {
    return _position;
}

auto CHcaCipherConverter::Read(
    void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count
) -> std::uint32_t {
    if (!buffer) {
        throw CArgumentException("CHcaCipherConverter::Read");
    }
    count = std::min(count, static_cast<std::uint32_t>(bufferSize - offset));
    if (count == 0) {
        return 0;
    }
    auto streamPosition     = GetPosition();
    const auto streamLength = GetLength();
    if (streamPosition >= streamLength) {
        return 0;
    }
    auto byteBuffer = static_cast<std::uint8_t *>(buffer);

    const auto &hcaInfo     = _hcaInfo;
    std::uint32_t totalRead = 0;
    if (streamPosition < hcaInfo.dataOffset) {
        const auto headerCopyLength =
            std::min(count, static_cast<std::uint32_t>(hcaInfo.dataOffset - streamPosition));
        const auto headerData = ConvertHeader();
        memcpy(byteBuffer + offset, headerData + streamPosition, headerCopyLength);
        streamPosition += headerCopyLength;
        if (count <= headerCopyLength) {
            SetPosition(streamPosition);
            return headerCopyLength;
        }
        count -= headerCopyLength;
        offset += headerCopyLength;
        totalRead += headerCopyLength;
    }

    while (count > 0 && streamPosition < streamLength) {
        const auto blockIndex =
            static_cast<std::uint32_t>((streamPosition - hcaInfo.dataOffset) / hcaInfo.blockSize);
        const auto startOffset = (streamPosition - hcaInfo.dataOffset) % hcaInfo.blockSize;
        const auto blockData   = ConvertBlock(blockIndex);
        const auto copyLength  = std::min(
            streamLength - streamPosition,
            std::min(
                static_cast<std::uint64_t>(count),
                static_cast<std::uint64_t>(hcaInfo.blockSize - startOffset)
            )
        );
        memcpy(byteBuffer + offset, blockData + startOffset, static_cast<std::size_t>(copyLength));
        streamPosition += copyLength;
        count -= copyLength;
        offset += copyLength;
        totalRead += copyLength;
    }

    SetPosition(streamPosition);
    return totalRead;
}

CGSS_NS_END
