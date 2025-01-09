#include <cstring>

#include "acb_env_ns.h"
#include "kawashima/hca/CHcaDecoderConfig.h"

ACB_NS_BEGIN

CHcaDecoderConfig::CHcaDecoderConfig() {
    std::memset(this, 0, sizeof(CHcaDecoderConfig));
}

ACB_NS_END
