#ifndef CGSS_TAKAMORI_STREAMS_ISTREAM_H_
#define CGSS_TAKAMORI_STREAMS_ISTREAM_H_

#include <cstddef>
#include <cstdint>

#include "cgss_enum.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

struct CGSS_EXPORT IStream {

    IStream(IStream &) = delete;

    virtual ~IStream() = default;

    virtual auto Read(
        void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count
    ) -> std::uint32_t PURE;

    virtual auto Write(
        const void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count
    ) -> std::uint32_t PURE;

    virtual void Seek(std::int64_t offset, StreamSeekOrigin origin) PURE;

    virtual auto IsWritable() const -> bool_t PURE;

    virtual auto IsReadable() const -> bool_t PURE;

    virtual auto IsSeekable() const -> bool_t PURE;

    virtual auto GetPosition() -> std::uint64_t PURE;

    virtual void SetPosition(std::uint64_t value) PURE;

    virtual auto GetLength() -> std::uint64_t PURE;

    virtual void SetLength(std::uint64_t value) PURE;

    virtual auto ReadByte() -> std::int32_t PURE;

    virtual auto WriteByte(std::uint8_t value) -> std::uint32_t PURE;

    virtual void Flush() PURE;

    virtual void CopyTo(IStream &destination) PURE;

    virtual void CopyTo(IStream &destination, std::uint32_t bufferSize) PURE;

protected:
    IStream() = default;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_STREAMS_ISTREAM_H_
