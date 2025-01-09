#ifndef ACB_TAKAMORI_STREAMS_CBINARYREADER_H_
#define ACB_TAKAMORI_STREAMS_CBINARYREADER_H_

#include <cstddef>
#include <cstdint>

#include "acb_env.h"
#include "acb_env_ns.h"

#include "./CStream.h"

ACB_NS_BEGIN

class ACB_EXPORT CBinaryReader: public CStream {

    __extends(CStream, CBinaryReader);

public:
    explicit CBinaryReader(IStream *baseStream);

    CBinaryReader(const CBinaryReader &) = delete;

    [[nodiscard]] auto ReadInt8() const -> std::int8_t;

    [[nodiscard]] auto ReadUInt8() const -> std::uint8_t;

    [[nodiscard]] auto ReadInt16LE() const -> std::int16_t;

    [[nodiscard]] auto ReadInt16BE() const -> std::int16_t;

    [[nodiscard]] auto ReadUInt16LE() const -> std::uint16_t;

    [[nodiscard]] auto ReadUInt16BE() const -> std::uint16_t;

    [[nodiscard]] auto ReadInt32LE() const -> std::int32_t;

    [[nodiscard]] auto ReadInt32BE() const -> std::int32_t;

    [[nodiscard]] auto ReadUInt32LE() const -> std::uint32_t;

    [[nodiscard]] auto ReadUInt32BE() const -> std::uint32_t;

    [[nodiscard]] auto ReadInt64LE() const -> std::int64_t;

    [[nodiscard]] auto ReadInt64BE() const -> std::int64_t;

    [[nodiscard]] auto ReadUInt64LE() const -> std::uint64_t;

    [[nodiscard]] auto ReadUInt64BE() const -> std::uint64_t;

    [[nodiscard]] auto ReadSingleLE() const -> float;

    [[nodiscard]] auto ReadSingleBE() const -> float;

    [[nodiscard]] auto ReadDoubleLE() const -> double;

    [[nodiscard]] auto ReadDoubleBE() const -> double;

    [[nodiscard]] auto ReadInt8(std::uint64_t offset) const -> std::int8_t;

    [[nodiscard]] auto ReadUInt8(std::uint64_t offset) const -> std::uint8_t;

    [[nodiscard]] auto ReadInt16LE(std::uint64_t offset) const -> std::int16_t;

    [[nodiscard]] auto ReadInt16BE(std::uint64_t offset) const -> std::int16_t;

    [[nodiscard]] auto ReadUInt16LE(std::uint64_t offset) const -> std::uint16_t;

    [[nodiscard]] auto ReadUInt16BE(std::uint64_t offset) const -> std::uint16_t;

    [[nodiscard]] auto ReadInt32LE(std::uint64_t offset) const -> std::int32_t;

    [[nodiscard]] auto ReadInt32BE(std::uint64_t offset) const -> std::int32_t;

    [[nodiscard]] auto ReadUInt32LE(std::uint64_t offset) const -> std::uint32_t;

    [[nodiscard]] auto ReadUInt32BE(std::uint64_t offset) const -> std::uint32_t;

    [[nodiscard]] auto ReadInt64LE(std::uint64_t offset) const -> std::int64_t;

    [[nodiscard]] auto ReadInt64BE(std::uint64_t offset) const -> std::int64_t;

    [[nodiscard]] auto ReadUInt64LE(std::uint64_t offset) const -> std::uint64_t;

    [[nodiscard]] auto ReadUInt64BE(std::uint64_t offset) const -> std::uint64_t;

    [[nodiscard]] auto ReadSingleLE(std::uint64_t offset) const -> float;

    [[nodiscard]] auto ReadSingleBE(std::uint64_t offset) const -> float;

    [[nodiscard]] auto ReadDoubleLE(std::uint64_t offset) const -> double;

    [[nodiscard]] auto ReadDoubleBE(std::uint64_t offset) const -> double;

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

    [[nodiscard]] auto PeekInt8() const -> std::int8_t;

    [[nodiscard]] auto PeekUInt8() const -> std::uint8_t;

    [[nodiscard]] auto PeekInt16LE() const -> std::int16_t;

    [[nodiscard]] auto PeekInt16BE() const -> std::int16_t;

    [[nodiscard]] auto PeekUInt16LE() const -> std::uint16_t;

    [[nodiscard]] auto PeekUInt16BE() const -> std::uint16_t;

    [[nodiscard]] auto PeekInt32LE() const -> std::int32_t;

    [[nodiscard]] auto PeekInt32BE() const -> std::int32_t;

    [[nodiscard]] auto PeekUInt32LE() const -> std::uint32_t;

    [[nodiscard]] auto PeekUInt32BE() const -> std::uint32_t;

    [[nodiscard]] auto PeekInt64LE() const -> std::int64_t;

    [[nodiscard]] auto PeekInt64BE() const -> std::int64_t;

    [[nodiscard]] auto PeekUInt64LE() const -> std::uint64_t;

    [[nodiscard]] auto PeekUInt64BE() const -> std::uint64_t;

    [[nodiscard]] auto PeekSingleLE() const -> float;

    [[nodiscard]] auto PeekSingleBE() const -> float;

    [[nodiscard]] auto PeekDoubleLE() const -> double;

    [[nodiscard]] auto PeekDoubleBE() const -> double;

    [[nodiscard]] auto PeekInt8(std::uint64_t offset) const -> std::int8_t;

    [[nodiscard]] auto PeekUInt8(std::uint64_t offset) const -> std::uint8_t;

    [[nodiscard]] auto PeekInt16LE(std::uint64_t offset) const -> std::int16_t;

    [[nodiscard]] auto PeekInt16BE(std::uint64_t offset) const -> std::int16_t;

    [[nodiscard]] auto PeekUInt16LE(std::uint64_t offset) const -> std::uint16_t;

    [[nodiscard]] auto PeekUInt16BE(std::uint64_t offset) const -> std::uint16_t;

    [[nodiscard]] auto PeekInt32LE(std::uint64_t offset) const -> std::int32_t;

    [[nodiscard]] auto PeekInt32BE(std::uint64_t offset) const -> std::int32_t;

    [[nodiscard]] auto PeekUInt32LE(std::uint64_t offset) const -> std::uint32_t;

    [[nodiscard]] auto PeekUInt32BE(std::uint64_t offset) const -> std::uint32_t;

    [[nodiscard]] auto PeekInt64LE(std::uint64_t offset) const -> std::int64_t;

    [[nodiscard]] auto PeekInt64BE(std::uint64_t offset) const -> std::int64_t;

    [[nodiscard]] auto PeekUInt64LE(std::uint64_t offset) const -> std::uint64_t;

    [[nodiscard]] auto PeekUInt64BE(std::uint64_t offset) const -> std::uint64_t;

    [[nodiscard]] auto PeekSingleLE(std::uint64_t offset) const -> float;

    [[nodiscard]] auto PeekSingleBE(std::uint64_t offset) const -> float;

    [[nodiscard]] auto PeekDoubleLE(std::uint64_t offset) const -> double;

    [[nodiscard]] auto PeekDoubleBE(std::uint64_t offset) const -> double;

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
        std::size_t bufferSize,
        std::size_t offset,
        std::size_t count
    ) -> std::size_t;

    auto Peek(void *buffer, std::size_t bufferSize, std::size_t bufferOffset, std::size_t count)
        -> std::size_t;

    auto Read(void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count)
        -> std::size_t override;

    auto Write(const void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count)
        -> std::size_t override;

    [[nodiscard]] auto IsWritable() const -> bool_t override;

    [[nodiscard]] auto IsReadable() const -> bool_t override;

    [[nodiscard]] auto IsSeekable() const -> bool_t override;

    auto GetPosition() -> std::uint64_t override;

    void SetPosition(std::uint64_t value) override;

    auto GetLength() -> std::uint64_t override;

    void SetLength(std::uint64_t value) override;

    void Flush() override;

private:
    IStream *_baseStream;
};

ACB_NS_END

#endif // ACB_TAKAMORI_STREAMS_CBINARYREADER_H_
