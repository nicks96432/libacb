#ifndef CGSS_TAKAMORI_STREAMS_CSTREAMEXTENSIONS_H_
#define CGSS_TAKAMORI_STREAMS_CSTREAMEXTENSIONS_H_

#include <cstddef>
#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./IStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CStreamExtensions {

public:
    CStreamExtensions() = delete;

    CStreamExtensions(const CStreamExtensions &) = delete;

    CStreamExtensions(CStreamExtensions &&) = delete;

    auto operator=(const CStreamExtensions &) -> CStreamExtensions & = delete;

    auto operator=(CStreamExtensions &&) -> CStreamExtensions & = delete;

    static void ReadNullEndedString(IStream *stream, std::string &buffer, std::size_t bufferSize);
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_STREAMS_CSTREAMEXTENSIONS_H_
