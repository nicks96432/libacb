#include <cstdint>
#include <cstring>

#include "acb_env_ns.h"
#include "kawashima/hca/CHcaCipherConfig.h"

ACB_NS_BEGIN

CHcaCipherConfig::CHcaCipherConfig(): MyBase() {
    std::memset(this, 0, sizeof(CHcaCipherConfig));
}

CHcaCipherConfig::CHcaCipherConfig(HcaCipherType cipherType): MyClass() {
    this->key.key = 0;
    if (cipherType == HcaCipherType::WithKey) {
        cipherType = HcaCipherType::NoCipher;
    }
    this->cipherType = cipherType;
}

CHcaCipherConfig::CHcaCipherConfig(std::uint32_t key1, std::uint32_t key2): MyClass() {
    Initialize(key1, key2, 0);
}

CHcaCipherConfig::CHcaCipherConfig(std::uint64_t key)
    : MyClass((std::uint32_t)(key >> 32u), (std::uint32_t)(key & 0xffffffff)) {}

CHcaCipherConfig::CHcaCipherConfig(
    std::uint32_t key1, std::uint32_t key2, std::uint16_t keyModifier
)
    : MyClass() {
    Initialize(key1, key2, keyModifier);
}

CHcaCipherConfig::CHcaCipherConfig(std::uint64_t key, std::uint16_t keyModifier)
    : MyClass((std::uint32_t)(key >> 32u), (std::uint32_t)(key & 0xffffffff), keyModifier) {}

void CHcaCipherConfig::Initialize(std::uint32_t key1, std::uint32_t key2, std::uint16_t modifier) {
    if (key1 == 0 && key2 == 0) {
        cipherType = HcaCipherType::NoCipher;
    } else {
        cipherType = HcaCipherType::WithKey;
    }
    this->key.keyParts.key1 = key1;
    this->key.keyParts.key2 = key2;
    this->keyModifier       = modifier;
}

ACB_NS_END
