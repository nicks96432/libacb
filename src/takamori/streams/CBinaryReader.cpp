#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>

#include "cgss_env_ns.h"
#include "common/quick_utils.h"
#include "takamori/exceptions/CException.h"
#include "takamori/exceptions/CInvalidOperationException.h"
#include "takamori/streams/CBinaryReader.h"
#include "takamori/streams/IStream.h"

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

CGSS_NS_BEGIN

CBinaryReader::CBinaryReader(IStream *baseStream): _baseStream(baseStream) {}

auto CBinaryReader::ReadInt8(IStream *stream) -> std::int8_t {
    std::uint8_t u = stream->ReadByte();
    std::int8_t i  = *(std::int8_t *)&u;
    return i;
}

auto CBinaryReader::ReadUInt8(IStream *stream) -> std::uint8_t {
    return stream->ReadByte();
}

#define ENSURE_READ(n)                                                        \
    static constexpr auto shouldRead = (n);                                   \
    std::array<std::uint8_t, shouldRead> buffer;                              \
    auto read = stream->Read(buffer.data(), buffer.size(), 0, buffer.size()); \
    do {                                                                      \
        if (read < buffer.size()) {                                           \
            throw CException(CGSS_OP_BUFFER_TOO_SMALL);                       \
        }                                                                     \
    } while (0)

#define READ_INT(bit, hostEndian)                                                  \
    ENSURE_READ((bit) / 8);                                                        \
    std::int##bit##_t ret = *reinterpret_cast<std::int##bit##_t *>(buffer.data()); \
    if (O32_HOST_ORDER != (hostEndian)) {                                          \
        ret = bswap(ret);                                                          \
    }                                                                              \
    return ret

#define READ_UINT(bit, hostEndian)                                                   \
    ENSURE_READ((bit) / 8);                                                          \
    std::uint##bit##_t ret = *reinterpret_cast<std::uint##bit##_t *>(buffer.data()); \
    if (O32_HOST_ORDER != (hostEndian)) {                                            \
        ret = bswap(ret);                                                            \
    }                                                                                \
    return ret

auto CBinaryReader::ReadInt16LE(IStream *stream) -> std::int16_t {
    READ_INT(16, O32_LITTLE_ENDIAN);
}

auto CBinaryReader::ReadInt16BE(IStream *stream) -> std::int16_t {
    READ_INT(16, O32_BIG_ENDIAN);
}

auto CBinaryReader::ReadUInt16LE(IStream *stream) -> std::uint16_t {
    READ_UINT(16, O32_LITTLE_ENDIAN);
}

auto CBinaryReader::ReadUInt16BE(IStream *stream) -> std::uint16_t {
    READ_UINT(16, O32_BIG_ENDIAN);
}

auto CBinaryReader::ReadInt32LE(IStream *stream) -> std::int32_t {
    READ_INT(32, O32_LITTLE_ENDIAN);
}

auto CBinaryReader::ReadInt32BE(IStream *stream) -> std::int32_t {
    READ_INT(32, O32_BIG_ENDIAN);
}

auto CBinaryReader::ReadUInt32LE(IStream *stream) -> std::uint32_t {
    READ_UINT(32, O32_LITTLE_ENDIAN);
}

auto CBinaryReader::ReadUInt32BE(IStream *stream) -> std::uint32_t {
    READ_UINT(32, O32_BIG_ENDIAN);
}

auto CBinaryReader::ReadInt64LE(IStream *stream) -> int64_t {
    READ_INT(64, O32_LITTLE_ENDIAN);
}

auto CBinaryReader::ReadInt64BE(IStream *stream) -> int64_t {
    READ_INT(64, O32_BIG_ENDIAN);
}

auto CBinaryReader::ReadUInt64LE(IStream *stream) -> std::uint64_t {
    READ_UINT(64, O32_LITTLE_ENDIAN);
}

auto CBinaryReader::ReadUInt64BE(IStream *stream) -> std::uint64_t {
    READ_UINT(64, O32_BIG_ENDIAN);
}

auto CBinaryReader::ReadSingleLE(IStream *stream) -> float {
    return std::bit_cast<float>(ReadInt32LE(stream));
}

auto CBinaryReader::ReadSingleBE(IStream *stream) -> float {
    return std::bit_cast<float>(ReadInt32BE(stream));
}

auto CBinaryReader::ReadDoubleLE(IStream *stream) -> double {
    return std::bit_cast<double>(ReadInt64LE(stream));
}

auto CBinaryReader::ReadDoubleBE(IStream *stream) -> double {
    return std::bit_cast<double>(ReadInt64BE(stream));
}

#define READ_U_FUNC_O(bit, suffix)                                                   \
    auto CBinaryReader::ReadUInt##bit##suffix(IStream *stream, std::uint64_t offset) \
        ->std::uint##bit##_t {                                                       \
        auto position = stream->GetPosition();                                       \
        stream->Seek(offset, StreamSeekOrigin::Begin);                               \
        auto value = ReadUInt##bit##suffix(stream);                                  \
        stream->SetPosition(position);                                               \
        return value;                                                                \
    }

#define READ_S_FUNC_O(bit, suffix)                                                  \
    auto CBinaryReader::ReadInt##bit##suffix(IStream *stream, std::uint64_t offset) \
        ->std::int##bit##_t {                                                       \
        auto position = stream->GetPosition();                                      \
        stream->Seek(offset, StreamSeekOrigin::Begin);                              \
        auto value = ReadInt##bit##suffix(stream);                                  \
        stream->SetPosition(position);                                              \
        return value;                                                               \
    }

READ_S_FUNC_O(8, )

READ_U_FUNC_O(8, )

READ_S_FUNC_O(16, LE)

READ_U_FUNC_O(16, LE)

READ_S_FUNC_O(32, LE)

READ_U_FUNC_O(32, LE)

READ_S_FUNC_O(64, LE)

READ_U_FUNC_O(64, LE)

READ_S_FUNC_O(16, BE)

READ_U_FUNC_O(16, BE)

READ_S_FUNC_O(32, BE)

READ_U_FUNC_O(32, BE)

READ_S_FUNC_O(64, BE)

READ_U_FUNC_O(64, BE)

#define READ_R_FUNC(type, Cap, suffix)                                                   \
    auto CBinaryReader::Read##Cap##suffix(IStream *stream, std::uint64_t offset)->type { \
        auto position = stream->GetPosition();                                           \
        stream->Seek(offset, StreamSeekOrigin::Begin);                                   \
        auto value = Read##Cap##suffix(stream);                                          \
        stream->SetPosition(position);                                                   \
        return value;                                                                    \
    }

READ_R_FUNC(float, Single, LE)

READ_R_FUNC(float, Single, BE)

READ_R_FUNC(double, Double, LE)

READ_R_FUNC(double, Double, BE)

#define PEEK_U_FUNC(bit, suffix)                                                     \
    auto CBinaryReader::PeekUInt##bit##suffix(IStream *stream)->std::uint##bit##_t { \
        auto position = stream->GetPosition();                                       \
        auto value    = ReadUInt##bit##suffix(stream);                               \
        stream->SetPosition(position);                                               \
        return value;                                                                \
    }

#define PEEK_S_FUNC(bit, suffix)                                                   \
    auto CBinaryReader::PeekInt##bit##suffix(IStream *stream)->std::int##bit##_t { \
        auto position = stream->GetPosition();                                     \
        auto value    = ReadInt##bit##suffix(stream);                              \
        stream->SetPosition(position);                                             \
        return value;                                                              \
    }

PEEK_S_FUNC(8, )

PEEK_U_FUNC(8, )

PEEK_S_FUNC(16, LE)

PEEK_U_FUNC(16, LE)

PEEK_S_FUNC(32, LE)

PEEK_U_FUNC(32, LE)

PEEK_S_FUNC(64, LE)

PEEK_U_FUNC(64, LE)

PEEK_S_FUNC(16, BE)

PEEK_U_FUNC(16, BE)

PEEK_S_FUNC(32, BE)

PEEK_U_FUNC(32, BE)

PEEK_S_FUNC(64, BE)

PEEK_U_FUNC(64, BE)

#define PEEK_R_FUNC(type, Cap, suffix)                             \
    auto CBinaryReader::Peek##Cap##suffix(IStream *stream)->type { \
        auto position = stream->GetPosition();                     \
        auto value    = Read##Cap##suffix(stream);                 \
        stream->SetPosition(position);                             \
        return value;                                              \
    }

PEEK_R_FUNC(float, Single, LE)

PEEK_R_FUNC(float, Single, BE)

PEEK_R_FUNC(double, Double, LE)

PEEK_R_FUNC(double, Double, BE)

#define READ_INSTANCE_WRAP_S(bit, suffix)                                 \
    auto CBinaryReader::ReadInt##bit##suffix() const->std::int##bit##_t { \
        return ReadInt##bit##suffix(_baseStream);                         \
    }

#define READ_INSTANCE_WRAP_U(bit, suffix)                                   \
    auto CBinaryReader::ReadUInt##bit##suffix() const->std::uint##bit##_t { \
        return ReadUInt##bit##suffix(_baseStream);                          \
    }

READ_INSTANCE_WRAP_S(8, )

READ_INSTANCE_WRAP_S(16, LE)

READ_INSTANCE_WRAP_S(16, BE)

READ_INSTANCE_WRAP_S(32, LE)

READ_INSTANCE_WRAP_S(32, BE)

READ_INSTANCE_WRAP_S(64, LE)

READ_INSTANCE_WRAP_S(64, BE)

READ_INSTANCE_WRAP_U(8, )

READ_INSTANCE_WRAP_U(16, LE)

READ_INSTANCE_WRAP_U(16, BE)

READ_INSTANCE_WRAP_U(32, LE)

READ_INSTANCE_WRAP_U(32, BE)

READ_INSTANCE_WRAP_U(64, LE)

READ_INSTANCE_WRAP_U(64, BE)

#define READ_INSTANCE_WRAP_S_O(bit, suffix)                                                   \
    auto CBinaryReader::ReadInt##bit##suffix(std::uint64_t offset) const->std::int##bit##_t { \
        return ReadInt##bit##suffix(_baseStream, offset);                                     \
    }

#define READ_INSTANCE_WRAP_U_O(bit, suffix)                                                     \
    auto CBinaryReader::ReadUInt##bit##suffix(std::uint64_t offset) const->std::uint##bit##_t { \
        return ReadUInt##bit##suffix(_baseStream, offset);                                      \
    }

READ_INSTANCE_WRAP_S_O(8, )

READ_INSTANCE_WRAP_S_O(16, LE)

READ_INSTANCE_WRAP_S_O(16, BE)

READ_INSTANCE_WRAP_S_O(32, LE)

READ_INSTANCE_WRAP_S_O(32, BE)

READ_INSTANCE_WRAP_S_O(64, LE)

READ_INSTANCE_WRAP_S_O(64, BE)

READ_INSTANCE_WRAP_U_O(8, )

READ_INSTANCE_WRAP_U_O(16, LE)

READ_INSTANCE_WRAP_U_O(16, BE)

READ_INSTANCE_WRAP_U_O(32, LE)

READ_INSTANCE_WRAP_U_O(32, BE)

READ_INSTANCE_WRAP_U_O(64, LE)

READ_INSTANCE_WRAP_U_O(64, BE)

#define PEEK_INSTANCE_WRAP_S(bit, suffix)                                 \
    auto CBinaryReader::PeekInt##bit##suffix() const->std::int##bit##_t { \
        return PeekInt##bit##suffix(_baseStream);                         \
    }

#define PEEK_INSTANCE_WRAP_U(bit, suffix)                                   \
    auto CBinaryReader::PeekUInt##bit##suffix() const->std::uint##bit##_t { \
        return PeekUInt##bit##suffix(_baseStream);                          \
    }

PEEK_INSTANCE_WRAP_S(8, )

PEEK_INSTANCE_WRAP_S(16, LE)

PEEK_INSTANCE_WRAP_S(16, BE)

PEEK_INSTANCE_WRAP_S(32, LE)

PEEK_INSTANCE_WRAP_S(32, BE)

PEEK_INSTANCE_WRAP_S(64, LE)

PEEK_INSTANCE_WRAP_S(64, BE)

PEEK_INSTANCE_WRAP_U(8, )

PEEK_INSTANCE_WRAP_U(16, LE)

PEEK_INSTANCE_WRAP_U(16, BE)

PEEK_INSTANCE_WRAP_U(32, LE)

PEEK_INSTANCE_WRAP_U(32, BE)

PEEK_INSTANCE_WRAP_U(64, LE)

PEEK_INSTANCE_WRAP_U(64, BE)

#define READ_INSTANCE_WRAP_R(type, Cap, suffix)           \
    auto CBinaryReader::Read##Cap##suffix() const->type { \
        return Read##Cap##suffix(_baseStream);            \
    }

READ_INSTANCE_WRAP_R(float, Single, LE)

READ_INSTANCE_WRAP_R(float, Single, BE)

READ_INSTANCE_WRAP_R(double, Double, LE)

READ_INSTANCE_WRAP_R(double, Double, BE)

#define READ_INSTANCE_WRAP_R_O(type, Cap, suffix)                             \
    auto CBinaryReader::Read##Cap##suffix(std::uint64_t offset) const->type { \
        return Read##Cap##suffix(_baseStream, offset);                        \
    }

READ_INSTANCE_WRAP_R_O(float, Single, LE)

READ_INSTANCE_WRAP_R_O(float, Single, BE)

READ_INSTANCE_WRAP_R_O(double, Double, LE)

READ_INSTANCE_WRAP_R_O(double, Double, BE)

#define PEEK_INSTANCE_WRAP_R(type, Cap, suffix)           \
    auto CBinaryReader::Peek##Cap##suffix() const->type { \
        return Peek##Cap##suffix(_baseStream);            \
    }

PEEK_INSTANCE_WRAP_R(float, Single, LE)

PEEK_INSTANCE_WRAP_R(float, Single, BE)

PEEK_INSTANCE_WRAP_R(double, Double, LE)

PEEK_INSTANCE_WRAP_R(double, Double, BE)

#define PEEK_INSTANCE_WRAP_R_O(type, Cap, suffix)                                              \
    auto CBinaryReader::Peek##Cap##suffix([[maybe_unused]] std::uint64_t offset) const->type { \
        return Peek##Cap##suffix(_baseStream);                                                 \
    }

PEEK_INSTANCE_WRAP_R_O(float, Single, LE)

PEEK_INSTANCE_WRAP_R_O(float, Single, BE)

PEEK_INSTANCE_WRAP_R_O(double, Double, LE)

PEEK_INSTANCE_WRAP_R_O(double, Double, BE)

#define PEEK_WRAP_S_O(bit, suffix)                                                  \
    auto CBinaryReader::PeekInt##bit##suffix(IStream *stream, std::uint64_t offset) \
        ->std::int##bit##_t {                                                       \
        auto position = stream->GetPosition();                                      \
        if (stream->GetPosition() != offset) {                                      \
            stream->Seek(offset, StreamSeekOrigin::Begin);                          \
        }                                                                           \
        auto value = ReadInt##bit##suffix(stream);                                  \
        stream->Seek(position, StreamSeekOrigin::Begin);                            \
        return value;                                                               \
    }

#define PEEK_WRAP_U_O(bit, suffix)                                                   \
    auto CBinaryReader::PeekUInt##bit##suffix(IStream *stream, std::uint64_t offset) \
        ->std::uint##bit##_t {                                                       \
        auto position = stream->GetPosition();                                       \
        if (stream->GetPosition() != offset) {                                       \
            stream->Seek(offset, StreamSeekOrigin::Begin);                           \
        }                                                                            \
        auto value = ReadUInt##bit##suffix(stream);                                  \
        stream->Seek(position, StreamSeekOrigin::Begin);                             \
        return value;                                                                \
    }

PEEK_WRAP_S_O(8, )

PEEK_WRAP_U_O(8, )

PEEK_WRAP_S_O(16, LE)

PEEK_WRAP_U_O(16, LE)

PEEK_WRAP_S_O(16, BE)

PEEK_WRAP_U_O(16, BE)

PEEK_WRAP_S_O(32, LE)

PEEK_WRAP_U_O(32, LE)

PEEK_WRAP_S_O(32, BE)

PEEK_WRAP_U_O(32, BE)

PEEK_WRAP_S_O(64, LE)

PEEK_WRAP_U_O(64, LE)

PEEK_WRAP_S_O(64, BE)

PEEK_WRAP_U_O(64, BE)

#define PEEK_INSTANCE_WRAP_S_O(bit, suffix)                                                   \
    auto CBinaryReader::PeekInt##bit##suffix(std::uint64_t offset) const->std::int##bit##_t { \
        return PeekInt##bit##suffix(_baseStream, offset);                                     \
    }

#define PEEK_INSTANCE_WRAP_U_O(bit, suffix)                                                     \
    auto CBinaryReader::PeekUInt##bit##suffix(std::uint64_t offset) const->std::uint##bit##_t { \
        return PeekUInt##bit##suffix(_baseStream, offset);                                      \
    }

PEEK_INSTANCE_WRAP_S_O(8, )

PEEK_INSTANCE_WRAP_S_O(16, LE)

PEEK_INSTANCE_WRAP_S_O(16, BE)

PEEK_INSTANCE_WRAP_S_O(32, LE)

PEEK_INSTANCE_WRAP_S_O(32, BE)

PEEK_INSTANCE_WRAP_S_O(64, LE)

PEEK_INSTANCE_WRAP_S_O(64, BE)

PEEK_INSTANCE_WRAP_U_O(8, )

PEEK_INSTANCE_WRAP_U_O(16, LE)

PEEK_INSTANCE_WRAP_U_O(16, BE)

PEEK_INSTANCE_WRAP_U_O(32, LE)

PEEK_INSTANCE_WRAP_U_O(32, BE)

PEEK_INSTANCE_WRAP_U_O(64, LE)

PEEK_INSTANCE_WRAP_U_O(64, BE)

#define PEEK_WRAP_R_O(type, Cap, suffix)                                                 \
    auto CBinaryReader::Peek##Cap##suffix(IStream *stream, std::uint64_t offset)->type { \
        auto position = stream->GetPosition();                                           \
        if (stream->GetPosition() != offset) {                                           \
            stream->Seek(offset, StreamSeekOrigin::Begin);                               \
        }                                                                                \
        auto value = Read##Cap##suffix(stream);                                          \
        stream->Seek(position, StreamSeekOrigin::Begin);                                 \
        return value;                                                                    \
    }

PEEK_WRAP_R_O(float, Single, LE)

PEEK_WRAP_R_O(float, Single, BE)

PEEK_WRAP_R_O(double, Double, LE)

PEEK_WRAP_R_O(double, Double, BE)

auto CBinaryReader::Peek(
    void *buffer, std::size_t bufferSize, std::size_t bufferOffset, std::size_t count
) -> std::size_t {
    return PeekBytes(
        _baseStream, static_cast<std::uint8_t *>(buffer), bufferSize, bufferOffset, count
    );
}

auto CBinaryReader::Read(
    void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
) -> std::size_t {
    return _baseStream->Read(buffer, bufferSize, offset, count);
}

auto CBinaryReader::Write(
    [[maybe_unused]] const void *buffer,
    [[maybe_unused]] std::size_t bufferSize,
    [[maybe_unused]] std::size_t offset,
    [[maybe_unused]] std::size_t count
) -> std::size_t {
    throw CInvalidOperationException("CBinaryReader::Write");
}

auto CBinaryReader::IsReadable() const -> bool_t {
    return _baseStream->IsReadable();
}

auto CBinaryReader::IsWritable() const -> bool_t {
    return FALSE;
}

auto CBinaryReader::IsSeekable() const -> bool_t {
    return _baseStream->IsSeekable();
}

auto CBinaryReader::GetPosition() -> std::uint64_t {
    return _baseStream->GetPosition();
}

void CBinaryReader::SetPosition(std::uint64_t value) {
    _baseStream->SetPosition(value);
}

auto CBinaryReader::GetLength() -> std::uint64_t {
    return _baseStream->GetLength();
}

void CBinaryReader::SetLength(std::uint64_t value) {
    _baseStream->SetLength(value);
}

void CBinaryReader::Flush() {
    throw CInvalidOperationException("CBinaryReader::Flush");
}

auto CBinaryReader::PeekBytes(
    IStream *stream,
    std::uint8_t *buffer,
    std::size_t bufferSize,
    std::size_t offset,
    std::size_t count
) -> std::size_t {
    const auto position = stream->GetPosition();
    const auto v        = stream->Read(buffer, bufferSize, offset, count);
    stream->Seek(position, StreamSeekOrigin::Begin);
    return v;
}

CGSS_NS_END
