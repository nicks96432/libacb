#ifndef ACB_TAKAMORI_STREAMS_CSTREAMEXTENSIONS_H_
#define ACB_TAKAMORI_STREAMS_CSTREAMEXTENSIONS_H_

#include <cstddef>
#include <string>

#include "acb_env.h"
#include "acb_env_ns.h"

#include "./IStream.h"

ACB_NS_BEGIN

class ACB_EXPORT CStreamExtensions {

public:
    CStreamExtensions() = delete;

    CStreamExtensions(const CStreamExtensions &) = delete;

    CStreamExtensions(CStreamExtensions &&) = delete;

    auto operator=(const CStreamExtensions &) -> CStreamExtensions & = delete;

    auto operator=(CStreamExtensions &&) -> CStreamExtensions & = delete;

    static void ReadNullEndedString(IStream *stream, std::string &buffer, std::size_t bufferSize);
};

ACB_NS_END

#endif // ACB_TAKAMORI_STREAMS_CSTREAMEXTENSIONS_H_
