#ifndef CGSS_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_
#define CGSS_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_

#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CException.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CNotImplementedException: public CException {

    __extends(CException, CNotImplementedException);

public:
    CNotImplementedException() noexcept;

    CNotImplementedException(const CNotImplementedException &) noexcept;

    explicit CNotImplementedException(const char *message) noexcept;

    explicit CNotImplementedException(const std::string &message) noexcept;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_EXCEPTIONS_CNOTIMPLEMENTEDEXCEPTION_H_
