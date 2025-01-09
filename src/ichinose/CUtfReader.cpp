#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>

#include "acb_env_ns.h"
#include "ichinose/CUtfReader.h"
#include "takamori/CBitConverter.h"
#include "takamori/streams/CBinaryReader.h"
#include "takamori/streams/IStream.h"

ACB_NS_BEGIN

CUtfReader::CUtfReader(): _encrypted(FALSE), _seed(0), _increment(0) {
    _currentXor             = 0;
    _currentUtfOffset       = 0;
    _currentStringXor       = 0;
    _currentUtfStringOffset = 0;
}

CUtfReader::CUtfReader(std::uint8_t seed, std::uint8_t increment)
    : _encrypted(TRUE), _seed(seed), _increment(increment) {
    _currentXor             = 0;
    _currentUtfOffset       = 0;
    _currentStringXor       = 0;
    _currentUtfStringOffset = 0;
}

auto CUtfReader::IsEncrypted() const -> bool_t {
    return _encrypted;
}

void CUtfReader::PeekBytes(
    IStream *stream,
    std::uint8_t *buffer,
    std::size_t streamOffset,
    std::size_t size,
    std::size_t utfOffset
) {
    PeekBytes(stream, buffer, 0, streamOffset, size, utfOffset);
}

void CUtfReader::PeekBytes(
    IStream *stream,
    std::uint8_t *buffer,
    std::size_t bufferOffset,
    std::size_t streamOffset,
    std::size_t size,
    std::size_t utfOffset
) {
    stream->Seek(streamOffset + utfOffset, StreamSeekOrigin::Begin);

    CBinaryReader::PeekBytes(stream, buffer, size, static_cast<std::size_t>(bufferOffset), size);

    if (!IsEncrypted()) {
        return;
    }

    if (utfOffset < _currentUtfOffset) {
        _currentUtfOffset = 0;
    }

    if (_currentUtfOffset == 0) {
        _currentXor = _seed;
    }

    for (auto j = _currentUtfOffset; j < utfOffset; ++j) {
        if (j > 0) {
            _currentXor *= _increment;
        }
        ++_currentUtfOffset;
    }

    for (std::size_t i = 0; i < size; ++i) {
        if (_currentUtfOffset != 0 || i > 0) {
            _currentXor *= _increment;
        }
        buffer[i] ^= _currentXor;
        ++_currentUtfOffset;
    }
}

auto CUtfReader::PeekUInt8(IStream *stream, std::size_t streamOffset, std::size_t utfOffset)
    -> std::uint8_t {
    auto value = CBinaryReader::PeekUInt8(stream, streamOffset + utfOffset);
    if (!IsEncrypted()) {
        return value;
    }
    if (utfOffset < _currentUtfOffset) {
        _currentUtfOffset = 0;
    }
    if (_currentUtfOffset == 0) {
        _currentXor = _seed;
    }
    for (auto j = _currentUtfOffset; j < utfOffset; ++j) {
        if (j > 0) {
            _currentXor *= _increment;
        }
        ++_currentUtfOffset;
    }
    if (_currentUtfOffset != 0) {
        _currentXor *= _increment;
    }
    value ^= _currentXor;
    ++_currentUtfOffset;
    return value;
}

auto CUtfReader::PeekInt8(IStream *stream, std::size_t streamOffset, std::size_t utfOffset)
    -> std::int8_t {
    const auto u = PeekUInt8(stream, streamOffset, utfOffset);
    return *(std::int8_t *)&u;
}

auto CUtfReader::PeekSingle(IStream *stream, std::size_t streamOffset, std::size_t utfOffset)
    -> float {
    std::int32_t i = PeekInt32(stream, streamOffset, utfOffset);
    return std::bit_cast<float>(i);
}

auto CUtfReader::PeekDouble(IStream *stream, std::size_t streamOffset, std::size_t utfOffset)
    -> double {
    std::int64_t i = PeekInt64(stream, streamOffset, utfOffset);
    return std::bit_cast<double>(i);
}

#define PEEK_FUNC(bit, u, U)                                                  \
    auto CUtfReader::Peek##U##Int##bit(                                       \
        IStream *stream, std::size_t streamOffset, std::size_t utfOffset      \
    )                                                                         \
        ->u##int##bit##_t {                                                   \
        std::array<std::uint8_t, ((bit) / 8)> temp;                           \
        PeekBytes(stream, temp.data(), streamOffset, temp.size(), utfOffset); \
        if (CBitConverter::IsLittleEndian()) {                                \
            std::reverse(temp.begin(), temp.end());                           \
        }                                                                     \
        return CBitConverter::To##U##Int##bit(temp.data());                   \
    }

PEEK_FUNC(16, , )

PEEK_FUNC(16, u, U)

PEEK_FUNC(32, , )

PEEK_FUNC(32, u, U)

PEEK_FUNC(64, , )

PEEK_FUNC(64, u, U)

ACB_NS_END
