#ifndef ACB_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_
#define ACB_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_

#include <string>

#include "acb_env.h"
#include "acb_env_ns.h"

#include "./CException.h"

ACB_NS_BEGIN

class CFormatException: public CException {

    __extends(CException, CFormatException);

public:
    ACB_EXPORT CFormatException() noexcept;

    ACB_EXPORT CFormatException(const CFormatException &) noexcept;

    ACB_EXPORT explicit CFormatException(const char *message) noexcept;

    ACB_EXPORT explicit CFormatException(const std::string &message) noexcept;
};

ACB_NS_END

#endif // ACB_TAKAMORI_EXCEPTIONS_CFORMATEXCEPTION_H_
