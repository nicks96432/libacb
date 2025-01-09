#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdint>
#include <cstring>

#include "cgss_env_ns.h"
#include "ichinose/CUtfField.h"

CGSS_NS_BEGIN

CUtfField::CUtfField() {
    std::memset(this, 0, sizeof(UTF_FIELD));
}

CUtfField::CUtfField(const UTF_FIELD &tpl) {
    std::memcpy(this, &tpl, sizeof(UTF_FIELD));

    std::memset(name, 0, sizeof(name));
    std::strncpy(name, tpl.name, UTF_FIELD_MAX_NAME_LEN);

    switch (type) {
    case CGSS_UTF_COLUMN_TYPE_DATA:
        if (tpl.value.data.ptr && tpl.value.data.size > 0) {
            value.data.ptr = new std::uint8_t[tpl.value.data.size];
            std::memcpy(value.data.ptr, tpl.value.data.ptr, tpl.value.data.size);
        }
        break;
    case CGSS_UTF_COLUMN_TYPE_STRING: {
        const auto strLength = tpl.value.str ? std::strlen(tpl.value.str) : 0;
        if (strLength > 0) {
            value.str = new char[strLength + 1];
            std::strncpy(value.str, tpl.value.str, strLength);
            value.str[strLength] = '\0';
        }
        break;
    }
    default:
        break;
    }
}

CUtfField::~CUtfField() {
    ReleaseData();
    std::memset(&value, 0, sizeof(value));
}

void CUtfField::SetName(const std::string &new_name) {
    std::strncpy(this->name, new_name.c_str(), UTF_FIELD_MAX_NAME_LEN);
    this->name[UTF_FIELD_MAX_NAME_LEN - 1] = '\0';
}

void CUtfField::ReleaseData() {
    switch (type) {
    case CGSS_UTF_COLUMN_TYPE_DATA:
        if (value.data.ptr) {
            delete[] value.data.ptr;
            value.data.ptr  = nullptr;
            value.data.size = 0;
        }
        break;
    case CGSS_UTF_COLUMN_TYPE_STRING:
        if (value.str) {
            delete[] value.str;
            value.str = nullptr;
        }
        break;
    default:
        break;
    }
}

void CUtfField::SetValue(
    const void *data, const std::uint32_t size, const std::uint32_t fieldOffset
) {
    ReleaseData();
    if (data && size > 0) {
        value.data.ptr = new std::uint8_t[size];
        std::memcpy(value.data.ptr, data, size);
    }
    value.data.size = size;
    offset          = fieldOffset;
    type            = CGSS_UTF_COLUMN_TYPE_DATA;
}

void CUtfField::SetValue(const char *str, std::uint32_t fieldOffset) {
    ReleaseData();
    if (str == nullptr) {
        return;
    }

    const auto strLength = std::strlen(str);

    value.str = new char[strLength + 1];
    std::memcpy(value.str, str, strLength);
    value.str[strLength] = '\0';

    offset = fieldOffset;
    type   = CGSS_UTF_COLUMN_TYPE_STRING;
}

void CUtfField::SetValue(const std::uint8_t v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.u8 = v;
    offset   = fieldOffset;
    type     = CGSS_UTF_COLUMN_TYPE_U8;
}

void CUtfField::SetValue(const std::int8_t v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.s8 = v;
    offset   = fieldOffset;
    type     = CGSS_UTF_COLUMN_TYPE_S8;
}

void CUtfField::SetValue(const std::uint16_t v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.u16 = v;
    offset    = fieldOffset;
    type      = CGSS_UTF_COLUMN_TYPE_U16;
}

void CUtfField::SetValue(const std::int16_t v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.s16 = v;
    offset    = fieldOffset;
    type      = CGSS_UTF_COLUMN_TYPE_S16;
}

void CUtfField::SetValue(const std::uint32_t v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.u32 = v;
    offset    = fieldOffset;
    type      = CGSS_UTF_COLUMN_TYPE_U32;
}

void CUtfField::SetValue(const std::int32_t v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.s32 = v;
    offset    = fieldOffset;
    type      = CGSS_UTF_COLUMN_TYPE_S32;
}

void CUtfField::SetValue(const std::uint64_t v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.u64 = v;
    offset    = fieldOffset;
    type      = CGSS_UTF_COLUMN_TYPE_U64;
}

void CUtfField::SetValue(const std::int64_t v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.s64 = v;
    offset    = fieldOffset;
    type      = CGSS_UTF_COLUMN_TYPE_S64;
}

void CUtfField::SetValue(const float v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.r32 = v;
    offset    = fieldOffset;
    type      = CGSS_UTF_COLUMN_TYPE_R32;
}

void CUtfField::SetValue(const double v, std::uint32_t fieldOffset) {
    ReleaseData();
    value.r64 = v;
    offset    = fieldOffset;
    type      = CGSS_UTF_COLUMN_TYPE_R64;
}

CGSS_NS_END
