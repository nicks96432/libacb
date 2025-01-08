#ifndef CGSS_ICHINOSE_CUTF_FIELD_H_
#define CGSS_ICHINOSE_CUTF_FIELD_H_

#include <cstdint>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CUtfField final: public UTF_FIELD {

    __extends(UTF_FIELD, CUtfField);

public:
    CUtfField();

    explicit CUtfField(const UTF_FIELD &tpl);

    ~CUtfField();

    void SetName(const char *name);

    void SetValue(const void *data, std::uint32_t size, std::uint32_t fieldOffset);

    void SetValue(const char *str, std::uint32_t fieldOffset);

    void SetValue(std::uint8_t v, std::uint32_t fieldOffset);

    void SetValue(std::int8_t v, std::uint32_t fieldOffset);

    void SetValue(std::uint16_t v, std::uint32_t fieldOffset);

    void SetValue(std::int16_t v, std::uint32_t fieldOffset);

    void SetValue(std::uint32_t v, std::uint32_t fieldOffset);

    void SetValue(std::int32_t v, std::uint32_t fieldOffset);

    void SetValue(std::uint64_t v, std::uint32_t fieldOffset);

    void SetValue(std::int64_t v, std::uint32_t fieldOffset);

    void SetValue(float v, std::uint32_t fieldOffset);

    void SetValue(double v, std::uint32_t fieldOffset);

private:
    void ReleaseData();
};

CGSS_NS_END

#endif // CGSS_ICHINOSE_CUTF_FIELD_H_
