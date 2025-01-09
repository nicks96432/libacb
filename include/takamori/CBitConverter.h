#ifndef ACB_TAKAMORI_CBITCONVERTER_H_
#define ACB_TAKAMORI_CBITCONVERTER_H_

#include <cstdint>

#include "acb_env.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

class ACB_EXPORT CBitConverter final {

    PURE_STATIC(CBitConverter);

public:
    static auto IsLittleEndian() -> bool_t;

    static auto ToInt16(const void *p) -> std::int16_t;

    static auto ToUInt16(const void *p) -> std::uint16_t;

    static auto ToInt32(const void *p) -> std::int32_t;

    static auto ToUInt32(const void *p) -> std::uint32_t;

    static auto ToInt64(const void *p) -> std::int64_t;

    static auto ToUInt64(const void *p) -> std::uint64_t;

    static auto ToSingle(const void *p) -> float;

    static auto ToDouble(const void *p) -> double;
};

ACB_NS_END

#endif // ACB_TAKAMORI_CBITCONVERTER_H_
