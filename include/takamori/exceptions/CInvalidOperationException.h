#ifndef ACB_TAKAMORI_EXCEPTIONS_CINVALIDOPERATIONEXCEPTION_H_
#define ACB_TAKAMORI_EXCEPTIONS_CINVALIDOPERATIONEXCEPTION_H_

#include <string>

#include "acb_env.h"
#include "acb_env_ns.h"

#include "./CException.h"

ACB_NS_BEGIN

class CInvalidOperationException: public CException {

    _extends(CException, CInvalidOperationException);

public:
    ACB_EXPORT CInvalidOperationException() noexcept;

    ACB_EXPORT CInvalidOperationException(const CInvalidOperationException &) noexcept;

    ACB_EXPORT explicit CInvalidOperationException(const char *message) noexcept;

    ACB_EXPORT explicit CInvalidOperationException(const std::string &message) noexcept;
};

ACB_NS_END

#endif // ACB_TAKAMORI_EXCEPTIONS_CINVALIDOPERATIONEXCEPTION_H_
