#ifndef ACB_ICHINOSE_CUTF_FIELD_H_
#define ACB_ICHINOSE_CUTF_FIELD_H_

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "acb_enum.h"
#include "acb_env.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

template<typename T>
concept valid_field_type = std::is_arithmetic_v<T> || std::is_same_v<std::string, T> ||
                           std::is_same_v<std::span<std::byte>, T>;

class CUtfField final {
public:
    ACB_EXPORT void SetName(const std::string &new_name);

    ACB_EXPORT void SetValue(valid_field_type auto v, std::uint32_t fieldOffset);

    UtfColumnType type{};
    UtfColumnStorage storage{};

    std::uint32_t offset{};
    std::uint32_t offsetInRow{};

    std::string name{};

    std::variant<
        std::uint8_t,
        std::int8_t,
        std::uint16_t,
        std::int16_t,
        std::uint32_t,
        std::int32_t,
        std::uint64_t,
        std::int64_t,
        float,
        double,
        std::vector<std::byte>,
        std::string>
        value{};
};

ACB_NS_END

#endif // ACB_ICHINOSE_CUTF_FIELD_H_
