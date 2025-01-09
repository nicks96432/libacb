#ifndef ACB_ICHINOSE_CACBHELPER_H_
#define ACB_ICHINOSE_CACBHELPER_H_

#include <cstddef>
#include <cstdint>

#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/streams/CMemoryStream.h"
#include "takamori/streams/IStream.h"

ACB_NS_BEGIN

class ACB_EXPORT CAcbHelper final {

    PURE_STATIC(CAcbHelper);

    __root_class(CAcbHelper);

public:
    static auto ExtractToNewStream(IStream *stream, std::size_t offset, std::size_t size)
        -> CMemoryStream *;

    static auto RoundUpToAlignment(std::uint64_t value, std::uint64_t alignment) -> std::uint64_t;

    static auto RoundUpToAlignment(std::uint32_t value, std::uint32_t alignment) -> std::uint32_t;

    static auto RoundUpToAlignment(std::int64_t value, std::int64_t alignment) -> std::int64_t;

    static auto RoundUpToAlignment(std::int32_t value, std::int32_t alignment) -> std::int32_t;
};

ACB_NS_END

#endif // ACB_ICHINOSE_CACBHELPER_H_
