#include <bit>
#include <cstddef>
#include <cstdint>

#include "acb_utils.h"
#include "takamori/exceptions/CInvalidOperationException.h"
#include "takamori/streams/CBinaryWriter.h"

// http://stackoverflow.com/questions/2100331/c-macro-definition-to-determine-big-endian-or-little-endian-machine
enum {
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN    = 0x00010203ul,
    O32_PDP_ENDIAN    = 0x01000302ul
};

static const union {
    std::uint8_t bytes[4]; // NOLINT(modernize-avoid-c-arrays)
    std::uint32_t value;
} _o32_host_order = {
    {0, 1, 2, 3}
};

#define O32_HOST_ORDER (_o32_host_order.value)

#define WRITE_XINT(bit, hostEndian)                                                       \
    if (O32_HOST_ORDER != (hostEndian)) {                                                 \
        v = bswap(v);                                                                     \
    }                                                                                     \
    std::uint8_t *buffer         = reinterpret_cast<std::uint8_t *>(&v);                  \
    static const auto bufferSize = (bit) / 8;                                             \
    auto written                 = _baseStream->Write(buffer, bufferSize, 0, bufferSize); \
    return written

ACB_NS_BEGIN

CBinaryWriter::CBinaryWriter(IStream *baseStream): _baseStream(baseStream) {}

auto CBinaryWriter::WriteInt8(std::int8_t v) -> std::size_t {
    return _baseStream->WriteByte(std::bit_cast<std::uint8_t>(v));
}

auto CBinaryWriter::WriteUInt8(std::uint8_t v) -> std::size_t {
    return _baseStream->WriteByte(v);
}

auto CBinaryWriter::WriteInt16LE(std::int16_t v) -> std::size_t {
    WRITE_XINT(16, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteInt16BE(std::int16_t v) -> std::size_t {
    WRITE_XINT(16, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteUInt16LE(std::uint16_t v) -> std::size_t {
    WRITE_XINT(16, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteUInt16BE(std::uint16_t v) -> std::size_t {
    WRITE_XINT(16, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteInt32LE(std::int32_t v) -> std::size_t {
    WRITE_XINT(32, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteInt32BE(std::int32_t v) -> std::size_t {
    WRITE_XINT(32, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteUInt32LE(std::uint32_t v) -> std::size_t {
    WRITE_XINT(32, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteUInt32BE(std::uint32_t v) -> std::size_t {
    WRITE_XINT(32, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteInt64LE(std::int64_t v) -> std::size_t {
    WRITE_XINT(64, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteInt64BE(std::int64_t v) -> std::size_t {
    WRITE_XINT(64, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteUInt64LE(std::uint64_t v) -> std::size_t {
    WRITE_XINT(64, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteUInt64BE(std::uint64_t v) -> std::size_t {
    WRITE_XINT(64, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteSingleLE(float v) -> std::size_t {
    return WriteInt32LE(std::bit_cast<std::int32_t>(v));
}

auto CBinaryWriter::WriteSingleBE(float v) -> std::size_t {
    return WriteInt32BE(std::bit_cast<std::int32_t>(v));
}

auto CBinaryWriter::WriteDoubleLE(double v) -> std::size_t {
    return WriteInt64LE(std::bit_cast<std::int64_t>(v));
}

auto CBinaryWriter::WriteDoubleBE(double v) -> std::size_t {
    return WriteInt64BE(std::bit_cast<std::int64_t>(v));
}

auto CBinaryWriter::Read(
    [[maybe_unused]] void *buffer,
    [[maybe_unused]] std::size_t bufferSize,
    [[maybe_unused]] std::size_t offset,
    [[maybe_unused]] std::size_t count
) -> std::size_t {
    throw CInvalidOperationException("CBinaryWriter::Read");
}

auto CBinaryWriter::Write(
    const void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
) -> std::size_t {
    return _baseStream->Write(buffer, bufferSize, offset, count);
}

auto CBinaryWriter::IsWritable() const -> bool_t {
    return _baseStream->IsWritable();
}

auto CBinaryWriter::IsReadable() const -> bool_t {
    return FALSE;
}

auto CBinaryWriter::IsSeekable() const -> bool_t {
    return _baseStream->IsSeekable();
}

auto CBinaryWriter::GetPosition() -> std::uint64_t {
    return _baseStream->GetPosition();
}

void CBinaryWriter::SetPosition(std::uint64_t value) {
    _baseStream->SetPosition(value);
}

auto CBinaryWriter::GetLength() -> std::uint64_t {
    return _baseStream->GetLength();
}

void CBinaryWriter::SetLength(std::uint64_t value) {
    _baseStream->SetLength(value);
}

void CBinaryWriter::Flush() {
    _baseStream->Flush();
}

ACB_NS_END
