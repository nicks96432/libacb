#ifndef ACB_KAWASHIMA_HCA_CDEFAULTWAVEGENERATOR_H_
#define ACB_KAWASHIMA_HCA_CDEFAULTWAVEGENERATOR_H_

#include <cstdint>

#include "acb_env.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

class ACB_EXPORT CDefaultWaveGenerator final {

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

ACB_NS_END

#endif // ACB_KAWASHIMA_HCA_CDEFAULTWAVEGENERATOR_H_
