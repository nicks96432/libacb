#ifndef ACB_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_
#define ACB_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_

#include <string>

#include "acb_env.h"
#include "acb_env_ns.h"

#include "./CException.h"

ACB_NS_BEGIN

class CNotImplementedException: public CException {

    _extends(CException, CNotImplementedException);

public:
    ACB_EXPORT CNotImplementedException() noexcept;

    ACB_EXPORT CNotImplementedException(const CNotImplementedException &) noexcept;

    ACB_EXPORT explicit CNotImplementedException(const char *message) noexcept;

    ACB_EXPORT explicit CNotImplementedException(const std::string &message) noexcept;
};

ACB_NS_END

#endif // ACB_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_
