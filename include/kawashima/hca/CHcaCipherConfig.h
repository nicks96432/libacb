#ifndef CGSS_KAWASHIMA_HCA_CHCACIPHERCONFIG_H_
#define CGSS_KAWASHIMA_HCA_CHCACIPHERCONFIG_H_

#include <cstdint>

#include "cgss_cdata.h"
#include "cgss_enum.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CHcaCipherConfig final: public HCA_CIPHER_CONFIG {

    __extends(HCA_CIPHER_CONFIG, CHcaCipherConfig);

public:
    CHcaCipherConfig();

    explicit CHcaCipherConfig(HcaCipherType cipherType);

    CHcaCipherConfig(std::uint32_t key1, std::uint32_t key2);

    explicit CHcaCipherConfig(std::uint64_t key);

    CHcaCipherConfig(std::uint32_t key1, std::uint32_t key2, std::uint16_t keyModifier);

    CHcaCipherConfig(std::uint64_t key, std::uint16_t keyModifier);

private:
    void Initialize(std::uint32_t key1, std::uint32_t key2, std::uint16_t modifier);
};

CGSS_NS_END

#endif // CGSS_KAWASHIMA_HCA_CHCACIPHERCONFIG_H_