#include <cstdint>

#include "ichinose/CAcbHelper.h"
#include "takamori/streams/CMemoryStream.h"

CGSS_NS_BEGIN

auto CAcbHelper::ExtractToNewStream(IStream *stream, std::uint64_t offset, std::uint32_t size)
    -> CMemoryStream * {
    const auto originalPosition = stream->GetPosition();

    stream->Seek(offset, StreamSeekOrigin::Begin);

    auto *memory              = new CMemoryStream(size, FALSE);
    auto bytesLeft            = size;
    constexpr auto bufferSize = 1024;
    auto *buffer              = new std::uint8_t[bufferSize];

    do {
        const auto read = stream->Read(buffer, bufferSize, 0, bytesLeft);

        memory->Write(buffer, bufferSize, 0, read);
        bytesLeft -= read;
    } while (bytesLeft > 0);

    delete[] buffer;
    stream->Seek(originalPosition, StreamSeekOrigin::Begin);
    memory->Seek(0, StreamSeekOrigin::Begin);

    return memory;
}

auto CAcbHelper::RoundUpToAlignment(std::uint64_t value, std::uint64_t alignment) -> std::uint64_t {
    return (value + alignment - 1) / alignment * alignment;
}

auto CAcbHelper::RoundUpToAlignment(std::uint32_t value, std::uint32_t alignment) -> std::uint32_t {
    return (value + alignment - 1) / alignment * alignment;
}

auto CAcbHelper::RoundUpToAlignment(std::int64_t value, std::int64_t alignment) -> std::int64_t {
    return (value + alignment - 1) / alignment * alignment;
}

auto CAcbHelper::RoundUpToAlignment(std::int32_t value, std::int32_t alignment) -> std::int32_t {
    return (value + alignment - 1) / alignment * alignment;
}

CGSS_NS_END
