#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <string>
#include <vector>

#include "acb_env_ns.h"
#include "ichinose/CUtfField.h"

ACB_NS_BEGIN

void CUtfField::SetName(const std::string &new_name) {
    this->name = new_name;
}

template<valid_field_type T>
void CUtfField::SetValue(T v, std::uint32_t fieldOffset) {
    this->offset = fieldOffset;

    if constexpr (std::is_same_v<decltype(v), std::span<std::byte>>) {
        this->value = std::vector<std::byte>{v.begin(), v.end()};
    } else {
        this->value = v;
    }

    if constexpr (std::is_same_v<decltype(v), std::int8_t>) {
        this->type = UtfColumnType::S8;
    } else if constexpr (std::is_same_v<decltype(v), std::uint8_t>) {
        this->type = UtfColumnType::U8;
    } else if constexpr (std::is_same_v<decltype(v), std::int16_t>) {
        this->type = UtfColumnType::S16;
    } else if constexpr (std::is_same_v<decltype(v), std::uint16_t>) {
        this->type = UtfColumnType::U16;
    } else if constexpr (std::is_same_v<decltype(v), std::int32_t>) {
        this->type = UtfColumnType::S32;
    } else if constexpr (std::is_same_v<decltype(v), std::uint32_t>) {
        this->type = UtfColumnType::U32;
    } else if constexpr (std::is_same_v<decltype(v), std::int64_t>) {
        this->type = UtfColumnType::S64;
    } else if constexpr (std::is_same_v<decltype(v), std::uint64_t>) {
        this->type = UtfColumnType::U64;
    } else if constexpr (std::is_same_v<decltype(v), float>) {
        this->type = UtfColumnType::R32;
    } else if constexpr (std::is_same_v<decltype(v), double>) {
        this->type = UtfColumnType::R64;
    } else if constexpr (std::is_same_v<decltype(v), std::string>) {
        this->type = UtfColumnType::String;
    } else {
        this->type = UtfColumnType::Data;
    }
}

template void CUtfField::SetValue<std::int8_t>(std::int8_t, std::uint32_t);
template void CUtfField::SetValue<std::uint8_t>(std::uint8_t, std::uint32_t);
template void CUtfField::SetValue<std::int16_t>(std::int16_t, std::uint32_t);
template void CUtfField::SetValue<std::uint16_t>(std::uint16_t, std::uint32_t);
template void CUtfField::SetValue<std::int32_t>(std::int32_t, std::uint32_t);
template void CUtfField::SetValue<std::uint32_t>(std::uint32_t, std::uint32_t);
template void CUtfField::SetValue<std::int64_t>(std::int64_t, std::uint32_t);
template void CUtfField::SetValue<std::uint64_t>(std::uint64_t, std::uint32_t);
template void CUtfField::SetValue<float>(float, std::uint32_t);
template void CUtfField::SetValue<double>(double, std::uint32_t);
template void CUtfField::SetValue<std::string>(std::string, std::uint32_t);
template void CUtfField::SetValue<std::span<std::byte>>(std::span<std::byte>, std::uint32_t);

ACB_NS_END
