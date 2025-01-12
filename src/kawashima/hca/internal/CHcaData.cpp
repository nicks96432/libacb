#include <array>
#include <cstddef>
#include <cstdint>

#include "acb_env_ns.h"

#include "./CHcaData.h"

ACB_NS_BEGIN

CHcaData::CHcaData(std::uint8_t *data, std::uint32_t dataSize, std::int32_t size) {
    _data     = data;
    _dataSize = dataSize;
    _size     = size * 8 - 16;
    _bit      = 0;
}

auto CHcaData::CheckBit(std::int32_t bitSize) -> std::int32_t {
#define SAFE_ACCESS(array, length, index) (((index) < (length)) ? (array)[(index)] : 0)

    std::int32_t v = 0;
    if (_bit + bitSize <= _size) {
        static std::array<std::int32_t, 8> mask = {
            0xFFFFFF, 0x7FFFFF, 0x3FFFFF, 0x1FFFFF, 0x0FFFFF, 0x07FFFF, 0x03FFFF, 0x01FFFF
        };
        std::size_t i = _bit >> 3;
        v             = SAFE_ACCESS(_data, _dataSize, i);
        v             = (v << 8) | SAFE_ACCESS(_data, _dataSize, i + 1);
        v             = (v << 8) | SAFE_ACCESS(_data, _dataSize, i + 2);
        v &= mask[_bit & 7];
        v >>= 24 - (_bit & 7) - static_cast<std::int32_t>(bitSize);
    }
    return v;

#undef SAFE_ACCESS
}

auto CHcaData::GetBit(std::int32_t bitSize) -> std::int32_t {
    std::int32_t v = CheckBit(bitSize);
    _bit += bitSize;
    return v;
}

void CHcaData::AddBit(std::int32_t bitSize) {
    _bit += bitSize;
}

ACB_NS_END
