#ifndef CGSS_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_
#define CGSS_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_

#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CException.h"

CGSS_NS_BEGIN

class CFormatException: public CException {

    __extends(CException, CFormatException);

public:
    CGSS_EXPORT CFormatException() noexcept;

    CGSS_EXPORT CFormatException(const CFormatException &) noexcept;

    CGSS_EXPORT explicit CFormatException(const char *message) noexcept;

    CGSS_EXPORT explicit CFormatException(const std::string &message) noexcept;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_
