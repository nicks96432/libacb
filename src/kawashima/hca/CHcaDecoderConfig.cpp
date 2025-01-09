#include <cstring>

#include "cgss_env_ns.h"
#include "kawashima/hca/CHcaDecoderConfig.h"

CGSS_NS_BEGIN

CHcaDecoderConfig::CHcaDecoderConfig() {
    std::memset(this, 0, sizeof(CHcaDecoderConfig));
}

CGSS_NS_END
