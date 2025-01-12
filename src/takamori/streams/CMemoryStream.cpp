#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>

// TODO: FILE: size_t vs. uint64

#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/exceptions/CArgumentException.h"
#include "takamori/exceptions/CInvalidOperationException.h"
#include "takamori/streams/CMemoryStream.h"

ACB_NS_BEGIN

static constexpr double MemoryStreamGrowFactor = 1.25;

CMemoryStream::CMemoryStream(): MyClass(0) {}

CMemoryStream::CMemoryStream(std::uint64_t capacity): MyClass(capacity, TRUE) {}

CMemoryStream::CMemoryStream(std::uint64_t capacity, bool_t isResizable) {
    auto buffer = new std::uint8_t[capacity];
    std::memset(buffer, 0, (std::size_t)capacity);
    _buffer           = buffer;
    _length           = 0;
    _capacity         = capacity;
    _isWritable       = TRUE;
    _isResizable      = isResizable;
    _isExternalBuffer = FALSE;
    _position         = 0;
}

CMemoryStream::CMemoryStream(std::uint8_t *buffer, std::uint64_t bufferSize)
    : MyClass(buffer, bufferSize, TRUE) {}

CMemoryStream::CMemoryStream(std::uint8_t *buffer, std::uint64_t bufferSize, bool_t isWritable) {
    _buffer = buffer;
    _length = _capacity = bufferSize;
    _isWritable         = isWritable;
    _isResizable        = FALSE;
    _isExternalBuffer   = TRUE;
    _position           = 0;
}

CMemoryStream::~CMemoryStream() {
    if (!IsExternalBuffer() && _buffer) {
        delete[] _buffer;
    }
    _buffer = nullptr;
}

auto CMemoryStream::Read(
    void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
) -> std::size_t {
    if (!buffer) {
        throw CArgumentException("MemoryStream::Read()");
    }
    if (!IsReadable()) {
        throw CInvalidOperationException("MemoryStream::Read()");
    }
    count               = std::min(bufferSize - offset, count);
    std::size_t maxRead = 0;
    if (count > 0) {
        auto position = GetPosition();
        maxRead       = static_cast<std::size_t>(
            std::min(GetLength() - position, static_cast<std::uint64_t>(count))
        );
        if (maxRead > 0) {
            const auto byteBuffer = static_cast<std::uint8_t *>(buffer);
            std::memcpy(byteBuffer + offset, _buffer + position, maxRead);
            position += maxRead;
            SetPosition(position);
        }
    }
    return static_cast<std::uint32_t>(maxRead);
}

auto CMemoryStream::Write(
    const void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
) -> std::size_t {
    if (!buffer) {
        throw CArgumentException("MemoryStream::Write()");
    }
    if (!IsWritable()) {
        throw CInvalidOperationException("MemoryStream::Write()");
    }
    count = std::min(bufferSize - offset, count);
    if (count > 0) {
        auto position             = GetPosition();
        const auto expectedLength = position + count;
        EnsureCapacity(expectedLength);
        const auto byteBuffer = static_cast<const std::uint8_t *>(buffer);
        std::memcpy(_buffer + position, byteBuffer + offset, count);
        position += count;
        if (position > GetLength()) {
            SetLength(position);
        }
        SetPosition(position);
    }
    return count;
}

auto CMemoryStream::IsWritable() const -> bool_t {
    return _isWritable;
}

auto CMemoryStream::IsReadable() const -> bool_t {
    return TRUE;
}

auto CMemoryStream::IsSeekable() const -> bool_t {
    return TRUE;
}

auto CMemoryStream::GetPosition() -> std::uint64_t {
    return _position;
}

void CMemoryStream::SetPosition(std::uint64_t value) {
    EnsureCapacity(value);
    _position = value;
}

auto CMemoryStream::GetLength() -> std::uint64_t {
    return _length;
}

void CMemoryStream::SetLength(std::uint64_t value) {
    EnsureCapacity(value);
    auto newLength = value;
    auto oldLength = _length;
    if (newLength < oldLength) {
        auto diff = (std::size_t)(oldLength - newLength);
        std::memset(_buffer + newLength, 0, diff);
    }
    _length = newLength;
}

void CMemoryStream::Flush() {
    // Do nothing.
}

auto CMemoryStream::GetCapacity() const -> std::uint64_t {
    return _capacity;
}

void CMemoryStream::SetCapacity(std::uint64_t value) {
    if (value == _capacity) {
        return;
    }
    if (!IsResizable()) {
        throw CInvalidOperationException("MemoryStream::SetCapacity()");
    }
    auto newBuffer   = new std::uint8_t[value];
    auto oldBuffer   = _buffer;
    auto length      = (std::size_t)GetLength();
    auto newCapacity = (std::size_t)value;
    length           = std::min(length, newCapacity);
    if (length > 0) {
        std::memcpy(newBuffer, oldBuffer, length);
        if (newCapacity > length) {
            auto diff = newCapacity - length;
            std::memset(newBuffer + length, 0, diff);
        }
    }
    _buffer = newBuffer;
    delete[] oldBuffer;
    _capacity = newCapacity;
}

void CMemoryStream::TrimExcess() {
    SetCapacity(GetLength());
}

auto CMemoryStream::GetBuffer() const -> std::uint8_t * {
    return _buffer;
}

auto CMemoryStream::ToArray() -> const std::uint8_t * {
    auto length = (std::size_t)GetLength();
    auto *p     = new std::uint8_t[length];
    std::memcpy(p, _buffer, length);
    return p;
}

auto CMemoryStream::IsResizable() const -> bool_t {
    return _isResizable;
}

auto CMemoryStream::IsExternalBuffer() const -> bool_t {
    return _isExternalBuffer;
}

void CMemoryStream::EnsureCapacity(std::uint64_t requestedLength) {
    auto capacity = GetCapacity();
    if (capacity >= requestedLength) {
        return;
    }
    if (!IsResizable()) {
        throw CInvalidOperationException("MemoryStream::EnsureCapacity()");
    }
    do {
        capacity =
            static_cast<std::uint64_t>(static_cast<double>(capacity) * MemoryStreamGrowFactor);
    } while (capacity < requestedLength);
    SetCapacity(capacity);
}

ACB_NS_END
