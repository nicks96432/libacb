#ifndef CGSS_KAWASHIMA_HCA_CHCAATH_H_
#define CGSS_KAWASHIMA_HCA_CHCAATH_H_

#include <array>
#include <cstdint>

#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CHcaAth {

public:
    CHcaAth();

    CHcaAth(const CHcaAth &) = default;

    auto Init(std::uint16_t type, std::uint32_t key) -> bool_t;

    auto GetTable() -> const std::uint8_t *;

private:
    std::array<std::uint8_t, 0x80> _table;

    void Init0();

    void Init1(std::uint32_t key);
};

CGSS_NS_END

#endif
