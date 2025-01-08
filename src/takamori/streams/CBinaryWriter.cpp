#include <cstddef>
#include <cstdint>

#include "common/quick_utils.h"
#include "takamori/exceptions/CInvalidOperationException.h"
#include "takamori/streams/CBinaryWriter.h"

// http://stackoverflow.com/questions/2100331/c-macro-definition-to-determine-big-endian-or-little-endian-machine
enum {
    O32_LITTLE_ENDIAN = 0x03020100ul,
    O32_BIG_ENDIAN    = 0x00010203ul,
    O32_PDP_ENDIAN    = 0x01000302ul
};

static const union {
    std::uint8_t bytes[4];
    std::uint32_t value;
} _o32_host_order = {
    {0, 1, 2, 3}
};

#define O32_HOST_ORDER (_o32_host_order.value)

#define WRITE_XINT(bit, hostEndian)                                                       \
    if (O32_HOST_ORDER != (hostEndian)) {                                                 \
        v = bswap(v);                                                                     \
    }                                                                                     \
    std::uint8_t *buffer         = (std::uint8_t *)&v;                                    \
    static const auto bufferSize = (bit) / 8;                                             \
    auto written                 = _baseStream->Write(buffer, bufferSize, 0, bufferSize); \
    return written

CGSS_NS_BEGIN

CBinaryWriter::CBinaryWriter(IStream *baseStream): _baseStream(baseStream) {}

auto CBinaryWriter::WriteInt8(std::int8_t v) -> std::uint32_t {
    std::uint8_t u = *(std::uint8_t *)&v;
    return _baseStream->WriteByte(u);
}

auto CBinaryWriter::WriteUInt8(std::uint8_t v) -> std::uint32_t {
    return _baseStream->WriteByte(v);
}

auto CBinaryWriter::WriteInt16LE(std::int16_t v) -> std::uint32_t {
    WRITE_XINT(16, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteInt16BE(std::int16_t v) -> std::uint32_t {
    WRITE_XINT(16, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteUInt16LE(std::uint16_t v) -> std::uint32_t {
    WRITE_XINT(16, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteUInt16BE(std::uint16_t v) -> std::uint32_t {
    WRITE_XINT(16, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteInt32LE(std::int32_t v) -> std::uint32_t {
    WRITE_XINT(32, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteInt32BE(std::int32_t v) -> std::uint32_t {
    WRITE_XINT(32, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteUInt32LE(std::uint32_t v) -> std::uint32_t {
    WRITE_XINT(32, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteUInt32BE(std::uint32_t v) -> std::uint32_t {
    WRITE_XINT(32, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteInt64LE(std::int64_t v) -> std::uint32_t {
    WRITE_XINT(64, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteInt64BE(std::int64_t v) -> std::uint32_t {
    WRITE_XINT(64, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteUInt64LE(std::uint64_t v) -> std::uint32_t {
    WRITE_XINT(64, O32_LITTLE_ENDIAN);
}

auto CBinaryWriter::WriteUInt64BE(std::uint64_t v) -> std::uint32_t {
    WRITE_XINT(64, O32_BIG_ENDIAN);
}

auto CBinaryWriter::WriteSingleLE(float v) -> std::uint32_t {
    std::int32_t i = *(std::int32_t *)&v;
    return WriteInt32LE(i);
}

auto CBinaryWriter::WriteSingleBE(float v) -> std::uint32_t {
    std::int32_t i = *(std::int32_t *)&v;
    return WriteInt32BE(i);
}

auto CBinaryWriter::WriteDoubleLE(double v) -> std::uint32_t {
    std::int64_t i = *(std::int64_t *)&v;
    return WriteInt64LE(i);
}

auto CBinaryWriter::WriteDoubleBE(double v) -> std::uint32_t {
    std::int64_t i = *(std::int64_t *)&v;
    return WriteInt64BE(i);
}

auto CBinaryWriter::Read(
    void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count
) -> std::uint32_t {
    throw CInvalidOperationException("CBinaryWriter::Read");
}

auto CBinaryWriter::Write(
    const void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count
) -> std::uint32_t {
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

CGSS_NS_END
