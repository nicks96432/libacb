#include <cstddef>
#include <cstdint>

#include "acb_env_ns.h"
#include "takamori/exceptions/CArgumentException.h"
#include "takamori/streams/CStream.h"
#include "takamori/streams/IStream.h"

ACB_NS_BEGIN

void CStream::Seek(std::int64_t offset, StreamSeekOrigin origin) {
    std::int64_t position = 0;
    switch (origin) {
    case StreamSeekOrigin::Begin:
        position = offset;
        break;
    case StreamSeekOrigin::Current:
        position = static_cast<std::int64_t>(GetPosition()) + offset;
        break;
    case StreamSeekOrigin::End:
        position = static_cast<std::int64_t>(GetLength()) + offset;
        break;
    }
    if (static_cast<std::uint64_t>(position) != GetPosition()) {
        SetPosition(static_cast<std::uint64_t>(position));
    }
}

auto CStream::ReadByte() -> std::uint8_t {
    std::uint8_t b = 0;
    Read(&b, 1, 0, 1);

    return b;
}

auto CStream::WriteByte(const std::uint8_t value) -> std::size_t {
    return Write(&value, 1, 0, 1);
}

void CStream::CopyTo(IStream &destination) {
    CopyTo(destination, 10240);
}

void CStream::CopyTo(IStream &destination, std::uint32_t bufferSize) {
    if (this == &destination) {
        throw CArgumentException("CStream::CopyTo");
    }
    if (bufferSize == 0) {
        return;
    }
    if (!IsReadable()) {
        return;
    }
    auto *buffer     = new std::uint8_t[bufferSize];
    std::size_t read = 1;
    while (read > 0) {
        read = Read(buffer, bufferSize, 0, bufferSize);
        if (read > 0) {
            destination.Write(buffer, bufferSize, 0, read);
        }
        if (read < bufferSize) {
            break;
        }
    }
    delete[] buffer;
}

ACB_NS_END
