#ifndef QUICK_UTILS_H_
#define QUICK_UTILS_H_

#include <cstdint>
#include <cstring>

#include "cgss_env_ns.h"

CGSS_NS_BEGIN

template<typename T>
void clone(const T &src, T &dst) {
    memcpy(&dst, &src, sizeof(T));
}

template<typename T>
auto clamp(T value, T min, T max) -> T {
    return value < min ? min : (value > max ? max : value);
}

inline auto bswap(std::uint16_t v) -> std::uint16_t {
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

inline auto bswap(std::int16_t v) -> std::int16_t {
    auto v2 = *(std::uint16_t *)&v;
    v2      = bswap(v2);
    return *(std::int16_t *)&v2;
}

inline auto bswap(std::uint32_t v) -> std::uint32_t {
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

inline auto bswap(std::int32_t v) -> std::int32_t {
    auto v2 = *(std::uint32_t *)&v;
    v2      = bswap(v2);
    return *(std::int32_t *)&v2;
}

inline auto bswap(std::uint64_t v) -> std::uint64_t {
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

inline auto bswap(std::int64_t v) -> std::int64_t {
    auto v2 = *(std::uint64_t *)&v;
    v2      = bswap(v2);
    return *(std::int64_t *)&v2;
}

inline auto bswap(float v) -> float {
    std::uint32_t i = bswap(*(std::uint32_t *)&v);
    return *(float *)&i;
}

inline auto bswap(double v) -> double {
    std::uint64_t i = bswap(*(std::uint64_t *)&v);
    return *(double *)&i;
}

inline auto ceil2(std::uint32_t a, std::uint32_t b) -> std::uint32_t {
    return (b > 0) ? (a / b + ((a % b) ? 1 : 0)) : 0;
}

CGSS_NS_END

#endif // QUICK_UTILS_H_
