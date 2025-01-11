#ifndef QUICK_UTILS_H_
#define QUICK_UTILS_H_

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "acb_env_ns.h"

ACB_NS_BEGIN

inline auto ceil2(std::uint32_t a, std::uint32_t b) -> std::uint32_t {
    return (b > 0) ? (a / b + ((a % b) ? 1 : 0)) : 0;
}

ACB_NS_END

#endif // QUICK_UTILS_H_
