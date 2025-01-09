#ifndef CGSS_KAWASHIMA_HCA_CHCADECODERCONFIG_H_
#define CGSS_KAWASHIMA_HCA_CHCADECODERCONFIG_H_

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CHcaDecoderConfig final: public HCA_DECODER_CONFIG {

    __extends(HCA_DECODER_CONFIG, CHcaDecoderConfig);

public:
    CHcaDecoderConfig();
};

CGSS_NS_END

#endif // CGSS_KAWASHIMA_HCA_CHCADECODERCONFIG_H_
