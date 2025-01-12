#ifndef ACB_KAWASHIMA_HCA_CHCADECODERCONFIG_H_
#define ACB_KAWASHIMA_HCA_CHCADECODERCONFIG_H_

#include "acb_cdata.h"
#include "acb_env.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

class ACB_EXPORT CHcaDecoderConfig final: public HCA_DECODER_CONFIG {

    _extends(HCA_DECODER_CONFIG, CHcaDecoderConfig);

public:
    CHcaDecoderConfig();
};

ACB_NS_END

#endif // ACB_KAWASHIMA_HCA_CHCADECODERCONFIG_H_
