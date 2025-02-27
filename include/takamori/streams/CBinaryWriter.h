#ifndef ACB_TAKAMORI_STREAMS_CBINARYWRITER_H_
#define ACB_TAKAMORI_STREAMS_CBINARYWRITER_H_

#include <cstddef>
#include <cstdint>

#include "acb_env.h"
#include "acb_env_ns.h"

#include "./CStream.h"

ACB_NS_BEGIN

class ACB_EXPORT CBinaryWriter: public CStream {

    _extends(CStream, CBinaryWriter);

public:
    explicit CBinaryWriter(IStream *baseStream);

    CBinaryWriter(const CBinaryWriter &) = delete;

    auto WriteInt8(std::int8_t v) -> std::size_t;

    auto WriteUInt8(std::uint8_t v) -> std::size_t;

    auto WriteInt16LE(std::int16_t v) -> std::size_t;

    auto WriteInt16BE(std::int16_t v) -> std::size_t;

    auto WriteUInt16LE(std::uint16_t v) -> std::size_t;

    auto WriteUInt16BE(std::uint16_t v) -> std::size_t;

    auto WriteInt32LE(std::int32_t v) -> std::size_t;

    auto WriteInt32BE(std::int32_t v) -> std::size_t;

    auto WriteUInt32LE(std::uint32_t v) -> std::size_t;

    auto WriteUInt32BE(std::uint32_t v) -> std::size_t;

    auto WriteInt64LE(std::int64_t v) -> std::size_t;

    auto WriteInt64BE(std::int64_t v) -> std::size_t;

    auto WriteUInt64LE(std::uint64_t v) -> std::size_t;

    auto WriteUInt64BE(std::uint64_t v) -> std::size_t;

    auto WriteSingleLE(float v) -> std::size_t;

    auto WriteSingleBE(float v) -> std::size_t;

    auto WriteDoubleLE(double v) -> std::size_t;

    auto WriteDoubleBE(double v) -> std::size_t;

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

#endif // ACB_TAKAMORI_STREAMS_CBINARYWRITER_H_
