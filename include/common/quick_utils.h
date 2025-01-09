#ifndef QUICK_UTILS_H_
#define QUICK_UTILS_H_

#include <bit>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <type_traits>

#include "cgss_env_ns.h"

CGSS_NS_BEGIN

inline auto _bswap(std::uint16_t v) -> std::uint16_t {
#if defined(__GNUC__)
    return __builtin_bswap16(v);
#elif defined(_MSC_VER)
    return _byteswap_ushort(v);
#else
    std::uint16_t r = (std::uint16_t)(v & 0xffu);
    r <<= 8;
    v >>= 8;
    r |= v & 0xffu;
    return r;
#endif
}

inline auto _bswap(std::uint32_t v) -> std::uint32_t {
#if defined(__GNUC__)
    return __builtin_bswap32(v);
#elif defined(_MSC_VER)
    return _byteswap_ulong(v);
#else
    std::uint32_t r = v & 0xffu;

    for (auto i = 0; i < 3; i += 1) {
        r <<= 8;
        v >>= 8;
        r |= v & 0xffu;
    }

    return r;
#endif
}

inline auto _bswap(std::uint64_t v) -> std::uint64_t {
#if defined(__GNUC__)
    return __builtin_bswap64(v);
#elif defined(_MSC_VER)
    return _byteswap_uint64(v);
#else
    std::uint64_t r = v & 0xffu;

    for (auto i = 0; i < 7; i += 1) {
        r <<= 8;
        v >>= 8;
        r |= v & 0xffu;
    }

    return r;
#endif
}

template<typename T>
[[nodiscard]] inline constexpr auto bswap(T v) -> T
    requires std::is_arithmetic_v<T> && (sizeof(T) >= 2)
{
    if constexpr (sizeof(T) == 8) {
        return std::bit_cast<T>(_bswap(std::bit_cast<std::uint64_t>(v)));
    } else if constexpr (sizeof(T) == 4) {
        return std::bit_cast<T>(_bswap(std::bit_cast<std::uint32_t>(v)));
    } else if constexpr (sizeof(T) == 2) {
        return std::bit_cast<T>(_bswap(std::bit_cast<std::uint16_t>(v)));
    } else {
        static_assert(false, "Unsupported type");
    }
}

inline auto ceil2(std::uint32_t a, std::uint32_t b) -> std::uint32_t {
    return (b > 0) ? (a / b + ((a % b) ? 1 : 0)) : 0;
}

CGSS_NS_END

#endif // QUICK_UTILS_H_
