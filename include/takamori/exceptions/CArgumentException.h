#ifndef ACB_TAKAMORI_EXCEPTIONS_CARGUMENTEXCEPTION_H_
#define ACB_TAKAMORI_EXCEPTIONS_CARGUMENTEXCEPTION_H_

#include <string>

#include "acb_env.h"
#include "acb_env_ns.h"

#include "./CException.h"

ACB_NS_BEGIN

class CArgumentException: public CException {

    __extends(CException, CArgumentException);

public:
    ACB_EXPORT CArgumentException() noexcept;

    ACB_EXPORT CArgumentException(const CArgumentException &) noexcept;

    ACB_EXPORT explicit CArgumentException(const char *message) noexcept;

    ACB_EXPORT explicit CArgumentException(const std::string &message) noexcept;
};

ACB_NS_END

#endif // ACB_TAKAMORI_EXCEPTIONS_CARGUMENTEXCEPTION_H_
