#ifndef CGSS_KAWASHIMA_HCA_CDEFAULTWAVEGENERATOR_H_
#define CGSS_KAWASHIMA_HCA_CDEFAULTWAVEGENERATOR_H_

#include <cstdint>

#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CDefaultWaveGenerator final {

public:
    static auto
    Decode8BitU(float data, std::uint8_t *buffer, std::uint32_t cursor) -> std::uint32_t;

    static auto
    Decode16BitS(float data, std::uint8_t *buffer, std::uint32_t cursor) -> std::uint32_t;

    static auto
    Decode24BitS(float data, std::uint8_t *buffer, std::uint32_t cursor) -> std::uint32_t;

    static auto
    Decode32BitS(float data, std::uint8_t *buffer, std::uint32_t cursor) -> std::uint32_t;

    static auto
    DecodeFloat(float data, std::uint8_t *buffer, std::uint32_t cursor) -> std::uint32_t;

    PURE_STATIC(CDefaultWaveGenerator);
};

CGSS_NS_END

#endif // CGSS_KAWASHIMA_HCA_CDEFAULTWAVEGENERATOR_H_
