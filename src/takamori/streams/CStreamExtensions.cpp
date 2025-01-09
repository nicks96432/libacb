#include <cstddef>
#include <cstdint>
#include <string>

#include "acb_env_ns.h"
#include "takamori/streams/CStreamExtensions.h"
#include "takamori/streams/IStream.h"

ACB_NS_BEGIN

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

ACB_NS_END
