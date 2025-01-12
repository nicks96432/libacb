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
    for (std::size_t i = 0; i < bufferSize; ++i) {
        std::uint8_t t = stream->ReadByte();
        if (!t) {
            return;
        }
        buffer += static_cast<char>(t);
    }
}

ACB_NS_END
