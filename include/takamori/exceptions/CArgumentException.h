#ifndef CGSS_TAKAMORI_EXCEPTIONS_CARGUMENTEXCEPTION_H_
#define CGSS_TAKAMORI_EXCEPTIONS_CARGUMENTEXCEPTION_H_

#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CException.h"

CGSS_NS_BEGIN

class CArgumentException: public CException {

    __extends(CException, CArgumentException);

public:
    CGSS_EXPORT CArgumentException() noexcept;

    CGSS_EXPORT CArgumentException(const CArgumentException &) noexcept;

    CGSS_EXPORT explicit CArgumentException(const char *message) noexcept;

    CGSS_EXPORT explicit CArgumentException(const std::string &message) noexcept;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_EXCEPTIONS_CARGUMENTEXCEPTION_H_
