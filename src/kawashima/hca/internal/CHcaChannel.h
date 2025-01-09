#ifndef ACB_KAWASHIMA_HCA_CHCACHANNEL_H_
#define ACB_KAWASHIMA_HCA_CHCACHANNEL_H_

#include <array>
#include <cstdint>

#include "acb_env_ns.h"

ACB_NS_BEGIN

class CHcaData;

class CHcaChannel {

public:
    CHcaChannel();

    CHcaChannel(const CHcaChannel &) = delete;

    static void Decode1(
        CHcaChannel *inst, CHcaData *data, std::uint32_t a, std::int32_t b, const std::uint8_t *ath
    );

    static void Decode2(CHcaChannel *inst, CHcaData *data);

    static void
    Decode3(CHcaChannel *inst, std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d);

    static void Decode4(
        CHcaChannel *inst1,
        CHcaChannel *inst2,
        std::int32_t index,
        std::uint32_t a,
        std::uint32_t b,
        std::uint32_t c
    );

    static void Decode5(CHcaChannel *inst, std::int32_t index);

    std::array<float, 0x80> block;
    std::array<float, 0x80> base;
    std::array<std::int8_t, 0x80> value;
    std::array<std::int8_t, 0x80> scale;
    std::array<std::int8_t, 8> value2;
    std::int32_t type;
    std::int8_t *value3;
    std::uint32_t count;
    std::array<float, 0x80> wav1;
    std::array<float, 0x80> wav2;
    std::array<float, 0x80> wav3;
    std::array<std::array<float, 0x80>, 8> wave;
};

ACB_NS_END

#endif // ACB_KAWASHIMA_HCA_CHCACHANNEL_H_
