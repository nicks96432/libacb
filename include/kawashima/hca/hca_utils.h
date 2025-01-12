#ifndef ACB_KAWASHIMA_HCA_HCAUTILS_H_
#define ACB_KAWASHIMA_HCA_HCAUTILS_H_

#include <cstdint>
#include <utility>

#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/exceptions/CFormatException.h"

ACB_NS_BEGIN

struct WaveSettings final {
    /**
     * Bit per channel. Future acceptable values will be 8, 16, 24, 32 and 0 (floating point wave
     * data).
     */
    static const std::uint32_t BitPerChannel;
    static const bool_t SoftLoop;

    PURE_STATIC(WaveSettings);
};

enum class Magic : std::uint32_t {

    HCA      = 0x00414348,
    FORMAT   = 0x00746D66,
    COMPRESS = 0x706D6F63,
    DECODE   = 0x00636564,
    VBR      = 0x00726276,
    ATH      = 0x00687461,
    LOOP     = 0x706F6F6C,
    CIPHER   = 0x68706963,
    RVA      = 0x00617672,
    COMMENT  = 0x6D6D6F63

};

inline auto areMagicMatch(std::uint32_t toCheck, Magic standard) -> bool_t {
    return static_cast<bool_t>((toCheck & 0x7f7f7f7f) == std::to_underlying(standard));
}

inline void ensureMagicMatch(std::uint32_t toCheck, Magic standard) {
    if (!areMagicMatch(toCheck, standard)) {
        throw CFormatException();
    }
}

ACB_NS_END

#endif // ACB_KAWASHIMA_HCA_HCAUTILS_H_
