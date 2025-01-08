#include <cstddef>
#include <cstdint>

#include "cgss_env_ns.h"
#include "takamori/streams/CStreamExtensions.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

void CStreamExtensions::ReadNullEndedString(IStream *stream, char *buffer, std::size_t bufferSize) {
    std::uint8_t t  = 1;
    std::uint32_t i = 0;

    while (t && i < bufferSize) {
        t = stream->ReadByte();

        buffer[i] = (char)t;

        ++i;
    }

    buffer[bufferSize - 1] = '\0';
}

CGSS_NS_END
