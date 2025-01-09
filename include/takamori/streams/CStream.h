#ifndef ACB_TAKAMORI_STREAMS_CSTREAM_H_
#define ACB_TAKAMORI_STREAMS_CSTREAM_H_

#include <cstddef>
#include <cstdint>

#include "acb_env.h"
#include "acb_env_ns.h"

#include "./IStream.h"

ACB_NS_BEGIN

class ACB_EXPORT CStream: public IStream {

public:
    CStream(const CStream &) = delete;

    ~CStream() override = default;

    void Seek(std::int64_t offset, StreamSeekOrigin origin) override;

    auto ReadByte() -> std::uint8_t override;

    auto WriteByte(std::uint8_t value) -> std::size_t override;

    void CopyTo(IStream &destination) override;

    void CopyTo(IStream &destination, std::uint32_t bufferSize) override;

protected:
    CStream() = default;
};

ACB_NS_END

#endif // ACB_TAKAMORI_STREAMS_CSTREAM_H_
