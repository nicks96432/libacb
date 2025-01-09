#ifndef CGSS_TAKAMORI_EXCEPTIONS_CINVALIDOPERATIONEXCEPTION_H_
#define CGSS_TAKAMORI_EXCEPTIONS_CINVALIDOPERATIONEXCEPTION_H_

#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CException.h"

CGSS_NS_BEGIN

class CInvalidOperationException: public CException {

    __extends(CException, CInvalidOperationException);

public:
    CGSS_EXPORT CInvalidOperationException() noexcept;

    CGSS_EXPORT CInvalidOperationException(const CInvalidOperationException &) noexcept;

    CGSS_EXPORT explicit CInvalidOperationException(const char *message) noexcept;

    CGSS_EXPORT explicit CInvalidOperationException(const std::string &message) noexcept;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_EXCEPTIONS_CINVALIDOPERATIONEXCEPTION_H_
