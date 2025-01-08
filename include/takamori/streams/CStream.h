#ifndef CGSS_TAKAMORI_STREAMS_CSTREAM_H_
#define CGSS_TAKAMORI_STREAMS_CSTREAM_H_

#include <cstdint>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./IStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CStream: public IStream {

public:
    CStream(const CStream &) = delete;

    ~CStream() override = default;

    void Seek(std::int64_t offset, StreamSeekOrigin origin) override;

    auto ReadByte() -> std::int32_t override;

    auto WriteByte(std::uint8_t value) -> std::uint32_t override;

    void CopyTo(IStream &destination) override;

    void CopyTo(IStream &destination, std::uint32_t bufferSize) override;

protected:
    CStream() = default;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_STREAMS_CSTREAM_H_
