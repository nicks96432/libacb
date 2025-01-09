#include <cstdint>

#include "kawashima/hca/CDefaultWaveGenerator.h"
#include "takamori/exceptions/CArgumentException.h"

CGSS_NS_BEGIN

auto CDefaultWaveGenerator::Decode8BitU(
    float data, std::uint8_t *buffer, const std::uint32_t cursor
) -> std::uint32_t {
    if (!buffer) {
        throw CArgumentException("CDefaultWaveGenerator::Decode8BitU");
    }
    auto u                             = (std::uint8_t)((std::int32_t)(data * 0x7f) + 0x80);
    *(std::uint8_t *)(buffer + cursor) = u;
    return cursor + 1;
}

auto CDefaultWaveGenerator::Decode16BitS(
    float data, std::uint8_t *buffer, const std::uint32_t cursor
) -> std::uint32_t {
    if (!buffer) {
        throw CArgumentException("CDefaultWaveGenerator::Decode16BitS");
    }
    auto i                             = (std::int16_t)(data * 0x7fff);
    *(std::int16_t *)(buffer + cursor) = i;
    return cursor + 2;
}

auto CDefaultWaveGenerator::Decode24BitS(
    float data, std::uint8_t *buffer, const std::uint32_t cursor
) -> std::uint32_t {
    if (!buffer) {
        throw CArgumentException("CDefaultWaveGenerator::Decode24BitS");
    }

    typedef struct _b24 {
        std::uint8_t a, b, c;
    } b24;

    typedef union _b32 {
        struct _Inner {
            b24 v;
            std::uint8_t a;
        };

        _Inner inner;
        std::int32_t dummy;
    } b32;

    b32 v;
    auto i                    = (std::int32_t)(data * 0x7fffff);
    v.dummy                   = i;
    *(b24 *)(buffer + cursor) = v.inner.v;
    return cursor + 3;
}

auto CDefaultWaveGenerator::Decode32BitS(
    float data, std::uint8_t *buffer, const std::uint32_t cursor
) -> std::uint32_t {
    if (!buffer) {
        throw CArgumentException("CDefaultWaveGenerator::Decode32BitS");
    }
    auto i                             = (std::int32_t)((double)data * 0x7fffffff);
    *(std::int32_t *)(buffer + cursor) = i;
    return cursor + 4;
}

auto CDefaultWaveGenerator::DecodeFloat(
    float data, std::uint8_t *buffer, const std::uint32_t cursor
) -> std::uint32_t {
    if (!buffer) {
        throw CArgumentException("CDefaultWaveGenerator::DecodeFloat");
    }
    *(float *)(buffer + cursor) = data;
    return cursor + 4;
}

CGSS_NS_END
