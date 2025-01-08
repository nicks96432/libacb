#ifndef CGSS_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_
#define CGSS_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_

#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CException.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CFormatException: public CException {

    __extends(CException, CFormatException);

public:
    CFormatException() noexcept;

    CFormatException(const CFormatException &) noexcept;

    explicit CFormatException(const char *message) noexcept;

    explicit CFormatException(const std::string &message) noexcept;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_
