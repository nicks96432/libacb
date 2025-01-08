#ifndef CGSS_TAKAMORI_STREAMS_CBINARYREADER_H_
#define CGSS_TAKAMORI_STREAMS_CBINARYREADER_H_

#include <cstddef>
#include <cstdint>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CBinaryReader: public CStream {

    __extends(CStream, CBinaryReader);

public:
    explicit CBinaryReader(IStream *baseStream);

    CBinaryReader(const CBinaryReader &) = delete;

    auto ReadInt8() const -> std::int8_t;

    auto ReadUInt8() const -> std::uint8_t;

    auto ReadInt16LE() const -> std::int16_t;

    auto ReadInt16BE() const -> std::int16_t;

    auto ReadUInt16LE() const -> std::uint16_t;

    auto ReadUInt16BE() const -> std::uint16_t;

    auto ReadInt32LE() const -> std::int32_t;

    auto ReadInt32BE() const -> std::int32_t;

    auto ReadUInt32LE() const -> std::uint32_t;

    auto ReadUInt32BE() const -> std::uint32_t;

    auto ReadInt64LE() const -> std::int64_t;

    auto ReadInt64BE() const -> std::int64_t;

    auto ReadUInt64LE() const -> std::uint64_t;

    auto ReadUInt64BE() const -> std::uint64_t;

    auto ReadSingleLE() const -> float;

    auto ReadSingleBE() const -> float;

    auto ReadDoubleLE() const -> double;

    auto ReadDoubleBE() const -> double;

    auto ReadInt8(std::uint64_t offset) const -> std::int8_t;

    auto ReadUInt8(std::uint64_t offset) const -> std::uint8_t;

    auto ReadInt16LE(std::uint64_t offset) const -> std::int16_t;

    auto ReadInt16BE(std::uint64_t offset) const -> std::int16_t;

    auto ReadUInt16LE(std::uint64_t offset) const -> std::uint16_t;

    auto ReadUInt16BE(std::uint64_t offset) const -> std::uint16_t;

    auto ReadInt32LE(std::uint64_t offset) const -> std::int32_t;

    auto ReadInt32BE(std::uint64_t offset) const -> std::int32_t;

    auto ReadUInt32LE(std::uint64_t offset) const -> std::uint32_t;

    auto ReadUInt32BE(std::uint64_t offset) const -> std::uint32_t;

    auto ReadInt64LE(std::uint64_t offset) const -> std::int64_t;

    auto ReadInt64BE(std::uint64_t offset) const -> std::int64_t;

    auto ReadUInt64LE(std::uint64_t offset) const -> std::uint64_t;

    auto ReadUInt64BE(std::uint64_t offset) const -> std::uint64_t;

    auto ReadSingleLE(std::uint64_t offset) const -> float;

    auto ReadSingleBE(std::uint64_t offset) const -> float;

    auto ReadDoubleLE(std::uint64_t offset) const -> double;

    auto ReadDoubleBE(std::uint64_t offset) const -> double;

    static auto ReadInt8(IStream *stream) -> std::int8_t;

    static auto ReadUInt8(IStream *stream) -> std::uint8_t;

    static auto ReadInt16LE(IStream *stream) -> std::int16_t;

    static auto ReadInt16BE(IStream *stream) -> std::int16_t;

    static auto ReadUInt16LE(IStream *stream) -> std::uint16_t;

    static auto ReadUInt16BE(IStream *stream) -> std::uint16_t;

    static auto ReadInt32LE(IStream *stream) -> std::int32_t;

    static auto ReadInt32BE(IStream *stream) -> std::int32_t;

    static auto ReadUInt32LE(IStream *stream) -> std::uint32_t;

    static auto ReadUInt32BE(IStream *stream) -> std::uint32_t;

    static auto ReadInt64LE(IStream *stream) -> std::int64_t;

    static auto ReadInt64BE(IStream *stream) -> std::int64_t;

    static auto ReadUInt64LE(IStream *stream) -> std::uint64_t;

    static auto ReadUInt64BE(IStream *stream) -> std::uint64_t;

    static auto ReadSingleLE(IStream *stream) -> float;

    static auto ReadSingleBE(IStream *stream) -> float;

    static auto ReadDoubleLE(IStream *stream) -> double;

    static auto ReadDoubleBE(IStream *stream) -> double;

    static auto ReadInt8(IStream *stream, std::uint64_t offset) -> std::int8_t;

    static auto ReadUInt8(IStream *stream, std::uint64_t offset) -> std::uint8_t;

    static auto ReadInt16LE(IStream *stream, std::uint64_t offset) -> std::int16_t;

    static auto ReadInt16BE(IStream *stream, std::uint64_t offset) -> std::int16_t;

    static auto ReadUInt16LE(IStream *stream, std::uint64_t offset) -> std::uint16_t;

    static auto ReadUInt16BE(IStream *stream, std::uint64_t offset) -> std::uint16_t;

    static auto ReadInt32LE(IStream *stream, std::uint64_t offset) -> std::int32_t;

    static auto ReadInt32BE(IStream *stream, std::uint64_t offset) -> std::int32_t;

    static auto ReadUInt32LE(IStream *stream, std::uint64_t offset) -> std::uint32_t;

    static auto ReadUInt32BE(IStream *stream, std::uint64_t offset) -> std::uint32_t;

    static auto ReadInt64LE(IStream *stream, std::uint64_t offset) -> std::int64_t;

    static auto ReadInt64BE(IStream *stream, std::uint64_t offset) -> std::int64_t;

    static auto ReadUInt64LE(IStream *stream, std::uint64_t offset) -> std::uint64_t;

    static auto ReadUInt64BE(IStream *stream, std::uint64_t offset) -> std::uint64_t;

    static auto ReadSingleLE(IStream *stream, std::uint64_t offset) -> float;

    static auto ReadSingleBE(IStream *stream, std::uint64_t offset) -> float;

    static auto ReadDoubleLE(IStream *stream, std::uint64_t offset) -> double;

    static auto ReadDoubleBE(IStream *stream, std::uint64_t offset) -> double;

    auto PeekInt8() const -> std::int8_t;

    auto PeekUInt8() const -> std::uint8_t;

    auto PeekInt16LE() const -> std::int16_t;

    auto PeekInt16BE() const -> std::int16_t;

    auto PeekUInt16LE() const -> std::uint16_t;

    auto PeekUInt16BE() const -> std::uint16_t;

    auto PeekInt32LE() const -> std::int32_t;

    auto PeekInt32BE() const -> std::int32_t;

    auto PeekUInt32LE() const -> std::uint32_t;

    auto PeekUInt32BE() const -> std::uint32_t;

    auto PeekInt64LE() const -> std::int64_t;

    auto PeekInt64BE() const -> std::int64_t;

    auto PeekUInt64LE() const -> std::uint64_t;

    auto PeekUInt64BE() const -> std::uint64_t;

    auto PeekSingleLE() const -> float;

    auto PeekSingleBE() const -> float;

    auto PeekDoubleLE() const -> double;

    auto PeekDoubleBE() const -> double;

    auto PeekInt8(std::uint64_t offset) const -> std::int8_t;

    auto PeekUInt8(std::uint64_t offset) const -> std::uint8_t;

    auto PeekInt16LE(std::uint64_t offset) const -> std::int16_t;

    auto PeekInt16BE(std::uint64_t offset) const -> std::int16_t;

    auto PeekUInt16LE(std::uint64_t offset) const -> std::uint16_t;

    auto PeekUInt16BE(std::uint64_t offset) const -> std::uint16_t;

    auto PeekInt32LE(std::uint64_t offset) const -> std::int32_t;

    auto PeekInt32BE(std::uint64_t offset) const -> std::int32_t;

    auto PeekUInt32LE(std::uint64_t offset) const -> std::uint32_t;

    auto PeekUInt32BE(std::uint64_t offset) const -> std::uint32_t;

    auto PeekInt64LE(std::uint64_t offset) const -> std::int64_t;

    auto PeekInt64BE(std::uint64_t offset) const -> std::int64_t;

    auto PeekUInt64LE(std::uint64_t offset) const -> std::uint64_t;

    auto PeekUInt64BE(std::uint64_t offset) const -> std::uint64_t;

    auto PeekSingleLE(std::uint64_t offset) const -> float;

    auto PeekSingleBE(std::uint64_t offset) const -> float;

    auto PeekDoubleLE(std::uint64_t offset) const -> double;

    auto PeekDoubleBE(std::uint64_t offset) const -> double;

    static auto PeekInt8(IStream *stream) -> std::int8_t;

    static auto PeekUInt8(IStream *stream) -> std::uint8_t;

    static auto PeekInt16LE(IStream *stream) -> std::int16_t;

    static auto PeekInt16BE(IStream *stream) -> std::int16_t;

    static auto PeekUInt16LE(IStream *stream) -> std::uint16_t;

    static auto PeekUInt16BE(IStream *stream) -> std::uint16_t;

    static auto PeekInt32LE(IStream *stream) -> std::int32_t;

    static auto PeekInt32BE(IStream *stream) -> std::int32_t;

    static auto PeekUInt32LE(IStream *stream) -> std::uint32_t;

    static auto PeekUInt32BE(IStream *stream) -> std::uint32_t;

    static auto PeekInt64LE(IStream *stream) -> std::int64_t;

    static auto PeekInt64BE(IStream *stream) -> std::int64_t;

    static auto PeekUInt64LE(IStream *stream) -> std::uint64_t;

    static auto PeekUInt64BE(IStream *stream) -> std::uint64_t;

    static auto PeekSingleLE(IStream *stream) -> float;

    static auto PeekSingleBE(IStream *stream) -> float;

    static auto PeekDoubleLE(IStream *stream) -> double;

    static auto PeekDoubleBE(IStream *stream) -> double;

    static auto PeekInt8(IStream *stream, std::uint64_t offset) -> std::int8_t;

    static auto PeekUInt8(IStream *stream, std::uint64_t offset) -> std::uint8_t;

    static auto PeekInt16LE(IStream *stream, std::uint64_t offset) -> std::int16_t;

    static auto PeekInt16BE(IStream *stream, std::uint64_t offset) -> std::int16_t;

    static auto PeekUInt16LE(IStream *stream, std::uint64_t offset) -> std::uint16_t;

    static auto PeekUInt16BE(IStream *stream, std::uint64_t offset) -> std::uint16_t;

    static auto PeekInt32LE(IStream *stream, std::uint64_t offset) -> std::int32_t;

    static auto PeekInt32BE(IStream *stream, std::uint64_t offset) -> std::int32_t;

    static auto PeekUInt32LE(IStream *stream, std::uint64_t offset) -> std::uint32_t;

    static auto PeekUInt32BE(IStream *stream, std::uint64_t offset) -> std::uint32_t;

    static auto PeekInt64LE(IStream *stream, std::uint64_t offset) -> std::int64_t;

    static auto PeekInt64BE(IStream *stream, std::uint64_t offset) -> std::int64_t;

    static auto PeekUInt64LE(IStream *stream, std::uint64_t offset) -> std::uint64_t;

    static auto PeekUInt64BE(IStream *stream, std::uint64_t offset) -> std::uint64_t;

    static auto PeekSingleLE(IStream *stream, std::uint64_t offset) -> float;

    static auto PeekSingleBE(IStream *stream, std::uint64_t offset) -> float;

    static auto PeekDoubleLE(IStream *stream, std::uint64_t offset) -> double;

    static auto PeekDoubleBE(IStream *stream, std::uint64_t offset) -> double;

    static auto PeekBytes(
        IStream *stream,
        std::uint8_t *buffer,
        std::uint32_t bufferSize,
        std::size_t offset,
        std::uint32_t count
    ) -> std::uint32_t;

    auto Peek(void *buffer, std::uint32_t bufferSize, std::size_t bufferOffset, std::uint32_t count)
        -> std::uint32_t;

    auto Read(void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count)
        -> std::uint32_t override;

    auto Write(
        const void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count
    ) -> std::uint32_t override;

    auto IsWritable() const -> bool_t override;

    auto IsReadable() const -> bool_t override;

    auto IsSeekable() const -> bool_t override;

    auto GetPosition() -> std::uint64_t override;

    void SetPosition(std::uint64_t value) override;

    auto GetLength() -> std::uint64_t override;

    void SetLength(std::uint64_t value) override;

    void Flush() override;

private:
    IStream *_baseStream;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_STREAMS_CBINARYREADER_H_
