#ifndef CGSS_TAKAMORI_STREAMS_CMEMORYSTREAM_H_
#define CGSS_TAKAMORI_STREAMS_CMEMORYSTREAM_H_

#include <cstddef>
#include <cstdint>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CMemoryStream final: public CStream {

    __extends(CStream, CMemoryStream);

public:
    CMemoryStream();

    explicit CMemoryStream(std::uint64_t capacity);

    CMemoryStream(std::uint64_t capacity, bool_t isResizable);

    CMemoryStream(std::uint8_t *buffer, std::uint64_t bufferSize);

    CMemoryStream(std::uint8_t *buffer, std::uint64_t bufferSize, bool_t isWritable);

    CMemoryStream(const CMemoryStream &) = delete;

    CMemoryStream(CMemoryStream &&) = delete;

    auto operator=(const CMemoryStream &) -> CMemoryStream & = delete;

    auto operator=(CMemoryStream &&) -> CMemoryStream & = delete;

    ~CMemoryStream() override;

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

    [[nodiscard]] virtual auto GetCapacity() const -> std::uint64_t;

    virtual void SetCapacity(std::uint64_t value);

    virtual void TrimExcess();

    [[nodiscard]] virtual auto GetBuffer() const -> std::uint8_t *;

    virtual auto ToArray() -> const std::uint8_t *;

private:
    [[nodiscard]] auto IsResizable() const -> bool_t;

    [[nodiscard]] auto IsExternalBuffer() const -> bool_t;

    void EnsureCapacity(std::uint64_t requestedLength);

private:
    std::uint8_t *_buffer;
    bool_t _isResizable;
    bool_t _isExternalBuffer;
    std::uint64_t _position;
    std::uint64_t _length;
    std::uint64_t _capacity;
    bool_t _isWritable;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_STREAMS_CMEMORYSTREAM_H_
