#include <cstddef>
#include <cstdint>
#include <cstring>

#include "cgss_cdata.h"

#include "./CHcaCipher.h"

static void TransformKey(
    std::uint32_t key1,
    std::uint32_t key2,
    std::uint16_t mod,
    std::uint32_t *pk1,
    std::uint32_t *pk2
) {
    auto key = (uint64_t)key2 << 32u | key1;
    auto k2  = ((uint64_t)mod << 16u | (std::uint16_t)(~mod + 2));

    auto newKey = key * k2;

    if (pk2) {
        *pk2 = static_cast<std::uint32_t>(newKey >> 32u);
    }

    if (pk1) {
        *pk1 = static_cast<std::uint32_t>(newKey & 0xffffffff);
    }
}

CGSS_NS_BEGIN

CHcaCipher::CHcaCipher() {
    Init(CHcaCipherConfig(HcaCipherType::NoCipher));
}

CHcaCipher::CHcaCipher(const HCA_CIPHER_CONFIG &config) {
    CHcaCipherConfig cfg(config.key, config.keyModifier);
    Init(cfg);
}

CHcaCipher::CHcaCipher(const CHcaCipherConfig &config) {
    Init(config);
}

CHcaCipher::CHcaCipher(const CHcaCipher &other) {
    _cipherType   = other._cipherType;
    _decryptTable = other._decryptTable;
    _encryptTable = other._encryptTable;
}

auto CHcaCipher::Init(const CHcaCipherConfig &config) -> bool_t {
    auto type = static_cast<HcaCipherType>(config.cipherType);

    if (config.key == 0 && type == HcaCipherType::WithKey) {
        type = HcaCipherType::NoCipher;
    }

    std::uint32_t key1 = config.keyParts.key1, key2 = config.keyParts.key2;

    if (config.key != 0 && type == HcaCipherType::WithKey && config.keyModifier) {
        TransformKey(key1, key2, config.keyModifier, &key1, &key2);
    }

    switch (type) {
    case HcaCipherType::NoCipher:
        Init0();
        break;
    case HcaCipherType::Static:
        Init1();
        break;
    case HcaCipherType::WithKey:
        Init56(key1, key2);
        break;
    }

    _cipherType = type;

    return InitEncryptTable();
}

void CHcaCipher::Decrypt(std::uint8_t *data, std::uint32_t size) const {
    for (std::uint8_t *d = data; size > 0; d++, size--) {
        *d = _decryptTable[*d];
    }
}

void CHcaCipher::Encrypt(std::uint8_t *data, std::uint32_t size) const {
    for (std::uint8_t *d = data; size > 0; d++, size--) {
        *d = _encryptTable[*d];
    }
}

void CHcaCipher::Init0() {
    for (std::uint32_t i = 0; i < TableSize; i++) {
        _decryptTable[i] = (std::uint8_t)i;
    }
}

void CHcaCipher::Init1() {
    for (std::uint32_t i = 1, v = 0; i < 0xFF; i++) {
        v = (v * 13u + 11u) & 0xFFu;
        if (v == 0 || v == 0xFF) {
            v = (v * 13u + 11u) & 0xFFu;
        }
        _decryptTable[i] = (std::uint8_t)v;
    }
    _decryptTable[0]    = 0;
    _decryptTable[0xFF] = 0xFF;
}

void CHcaCipher::Init56(std::uint32_t key1, std::uint32_t key2) {
    // Generate table #1
    std::array<std::uint8_t, 8> t1;
    if (!key1) {
        key2--;
    }
    key1--;
    for (std::uint8_t i = 0; i < 7; i++) {
        t1[i] = (std::uint8_t)key1;
        key1  = (key1 >> 8u) | (key2 << 24u);
        key2 >>= 8u;
    }

    // Generate table #2
    std::array<std::uint8_t, 0x10> t2 = {
        t1[1],
        (std::uint8_t)(t1[1] ^ t1[6]),
        (std::uint8_t)(t1[2] ^ t1[3]),
        t1[2],
        (std::uint8_t)(t1[2] ^ t1[1]),
        (std::uint8_t)(t1[3] ^ t1[4]),
        t1[3],
        (std::uint8_t)(t1[3] ^ t1[2]),
        (std::uint8_t)(t1[4] ^ t1[5]),
        t1[4],
        (std::uint8_t)(t1[4] ^ t1[3]),
        (std::uint8_t)(t1[5] ^ t1[6]),
        t1[5],
        (std::uint8_t)(t1[5] ^ t1[4]),
        (std::uint8_t)(t1[6] ^ t1[1]),
        t1[6],
    };

    // Generate table #3
    std::array<std::uint8_t, 0x100> t3;
    std::array<std::uint8_t, 0x10> t31, t32;
    auto t = t3.begin();
    Init56_CreateTable(t31, t1[0]);
    for (std::size_t i = 0; i < t31.size(); i++) {
        Init56_CreateTable(t32, t2[i]);
        std::uint8_t v = t31[i] << 4u;

        for (unsigned char j : t32) {
            *(t++) = v | j;
        }
    }

    // Generate CIPH table
    t = &_decryptTable[1];
    for (std::uint32_t i = 0, v = 0; i < TableSize; i++) {
        v              = (v + 0x11u) & 0xFFu;
        std::uint8_t a = t3[v];
        if (a != 0 && a != 0xFFu) {
            *(t++) = a;
        }
    }
    _decryptTable[0]    = 0;
    _decryptTable[0xFF] = 0xFF;
}

void CHcaCipher::Init56_CreateTable(std::array<std::uint8_t, 0x10> &r, std::uint8_t key) {
    std::uint32_t mul = ((key & 1u) << 3u) | 5u;
    std::uint32_t add = (key & 0xEu) | 1u;
    key >>= 4u;

    for (auto &i : r) {
        key = static_cast<std::uint8_t>((key * mul + add) & 0xFu);
        i   = key;
    }
}

auto CHcaCipher::InitEncryptTable() -> bool_t {
    _encryptTable.fill(0);
    for (std::uint32_t i = 0; i < TableSize; ++i) {
        _encryptTable[_decryptTable[i]] = (std::uint8_t)i;
    }
    return TRUE;
}

CGSS_NS_END
