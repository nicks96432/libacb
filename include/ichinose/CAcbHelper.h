#ifndef CGSS_ICHINOSE_CACBHELPER_H_
#define CGSS_ICHINOSE_CACBHELPER_H_

#include <cstdint>

#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "takamori/streams/CMemoryStream.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CAcbHelper final {

    PURE_STATIC(CAcbHelper);

    __root_class(CAcbHelper);

public:
    static auto ExtractToNewStream(IStream *stream, std::uint64_t offset, std::uint32_t size)
        -> CMemoryStream *;

    static auto RoundUpToAlignment(std::uint64_t value, std::uint64_t alignment) -> std::uint64_t;

    static auto RoundUpToAlignment(std::uint32_t value, std::uint32_t alignment) -> std::uint32_t;

    static auto RoundUpToAlignment(std::int64_t value, std::int64_t alignment) -> std::int64_t;

    static auto RoundUpToAlignment(std::int32_t value, std::int32_t alignment) -> std::int32_t;
};

CGSS_NS_END

#endif // CGSS_ICHINOSE_CACBHELPER_H_
