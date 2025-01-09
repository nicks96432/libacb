#ifndef ACB_KAWASHIMA_HCA_CHCADATA_H_
#define ACB_KAWASHIMA_HCA_CHCADATA_H_

#include <cstdint>

#include "acb_env_ns.h"

ACB_NS_BEGIN

class CHcaData {

public:
    CHcaData(std::uint8_t *data, std::uint32_t dataSize, std::uint32_t size);

    CHcaData(CHcaData &) = default;

    auto CheckBit(std::int32_t bitSize) -> std::int32_t;

    auto GetBit(std::int32_t bitSize) -> std::int32_t;

    void AddBit(std::int32_t bitSize);

private:
    std::uint8_t *_data;
    std::uint32_t _dataSize;
    std::int32_t _size;
    std::int32_t _bit;
};

ACB_NS_END

#endif // ACB_KAWASHIMA_HCA_CHCADATA_H_
