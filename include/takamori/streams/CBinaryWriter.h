#ifndef CGSS_TAKAMORI_STREAMS_CBINARYWRITER_H_
#define CGSS_TAKAMORI_STREAMS_CBINARYWRITER_H_

#include <cstddef>
#include <cstdint>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CBinaryWriter: public CStream {

    __extends(CStream, CBinaryWriter);

public:
    explicit CBinaryWriter(IStream *baseStream);

    CBinaryWriter(const CBinaryWriter &) = delete;

    auto WriteInt8(std::int8_t v) -> std::uint32_t;

    auto WriteUInt8(std::uint8_t v) -> std::uint32_t;

    auto WriteInt16LE(std::int16_t v) -> std::uint32_t;

    auto WriteInt16BE(std::int16_t v) -> std::uint32_t;

    auto WriteUInt16LE(std::uint16_t v) -> std::uint32_t;

    auto WriteUInt16BE(std::uint16_t v) -> std::uint32_t;

    auto WriteInt32LE(std::int32_t v) -> std::uint32_t;

    auto WriteInt32BE(std::int32_t v) -> std::uint32_t;

    auto WriteUInt32LE(std::uint32_t v) -> std::uint32_t;

    auto WriteUInt32BE(std::uint32_t v) -> std::uint32_t;

    auto WriteInt64LE(std::int64_t v) -> std::uint32_t;

    auto WriteInt64BE(std::int64_t v) -> std::uint32_t;

    auto WriteUInt64LE(std::uint64_t v) -> std::uint32_t;

    auto WriteUInt64BE(std::uint64_t v) -> std::uint32_t;

    auto WriteSingleLE(float v) -> std::uint32_t;

    auto WriteSingleBE(float v) -> std::uint32_t;

    auto WriteDoubleLE(double v) -> std::uint32_t;

    auto WriteDoubleBE(double v) -> std::uint32_t;

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

#endif // CGSS_TAKAMORI_STREAMS_CBINARYWRITER_H_
