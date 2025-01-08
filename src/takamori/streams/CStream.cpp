#include <cstdint>

#include "cgss_env_ns.h"
#include "takamori/exceptions/CArgumentException.h"
#include "takamori/streams/CStream.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

void CStream::Seek(std::int64_t offset, StreamSeekOrigin origin) {
    std::int64_t position;
    switch (origin) {
    case StreamSeekOrigin::Begin:
        position = offset;
        break;
    case StreamSeekOrigin::Current:
        position = GetPosition() + offset;
        break;
    case StreamSeekOrigin::End:
        position = GetLength() + offset;
        break;
    }
    if (position != GetPosition()) {
        SetPosition((std::uint64_t)position);
    }
}

auto CStream::ReadByte() -> std::int32_t {
    std::uint8_t b = 0;
    auto read      = Read(&b, 1, 0, 1);
    if (read < 1) {
        b = -1;
    }
    return b;
}

auto CStream::WriteByte(const std::uint8_t value) -> std::uint32_t {
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
    auto *buffer       = new std::uint8_t[bufferSize];
    std::uint32_t read = 1;
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

CGSS_NS_END
