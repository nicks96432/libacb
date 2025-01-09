#include <cstddef>
#include <cstdint>
#include <string>

#include "cgss_env_ns.h"
#include "takamori/streams/CStreamExtensions.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

void CStreamExtensions::ReadNullEndedString(
    IStream *stream, std::string &buffer, std::size_t bufferSize
) {
    std::uint8_t t = 1;
    std::size_t i  = 0;

    while (t && i < bufferSize) {
        t = stream->ReadByte();

        buffer += static_cast<char>(t);

        ++i;
    }
}

CGSS_NS_END
