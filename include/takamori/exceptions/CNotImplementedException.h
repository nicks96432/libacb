#ifndef CGSS_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_
#define CGSS_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_

#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CException.h"

CGSS_NS_BEGIN

class CNotImplementedException: public CException {

    __extends(CException, CNotImplementedException);

public:
    CGSS_EXPORT CNotImplementedException() noexcept;

    CGSS_EXPORT CNotImplementedException(const CNotImplementedException &) noexcept;

    CGSS_EXPORT explicit CNotImplementedException(const char *message) noexcept;

    CGSS_EXPORT explicit CNotImplementedException(const std::string &message) noexcept;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_
