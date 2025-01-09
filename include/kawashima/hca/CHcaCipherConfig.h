#ifndef ACB_KAWASHIMA_HCA_CHCACIPHERCONFIG_H_
#define ACB_KAWASHIMA_HCA_CHCACIPHERCONFIG_H_

#include <cstdint>

#include "acb_cdata.h"
#include "acb_enum.h"
#include "acb_env.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

class ACB_EXPORT CHcaCipherConfig final: public HCA_CIPHER_CONFIG {

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

ACB_NS_END

#endif // ACB_KAWASHIMA_HCA_CHCACIPHERCONFIG_H_