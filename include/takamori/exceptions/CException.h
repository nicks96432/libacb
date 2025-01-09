#ifndef CGSS_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_
#define CGSS_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_

#include <exception>
#include <string>

#include "cgss_cenum.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "cgss_env_platform.h"

CGSS_NS_BEGIN

class CException: public std::exception {

    __extends(std::exception, CException);

public:
    CGSS_EXPORT CException() noexcept;

    CGSS_EXPORT CException(const CException &) noexcept;

    CGSS_EXPORT auto operator=(const CException &) noexcept -> CException &;

    CGSS_EXPORT explicit CException(CGSS_OP_RESULT result) noexcept;

    CGSS_EXPORT explicit CException(const char *message) noexcept;

    CGSS_EXPORT explicit CException(const std::string &message) noexcept;

    CGSS_EXPORT CException(CGSS_OP_RESULT result, std::string message) noexcept;

    CGSS_EXPORT ~CException() noexcept override = default;

    [[nodiscard]] CGSS_EXPORT virtual auto
    GetExceptionMessage() const noexcept -> const std::string &;

    [[nodiscard]] CGSS_EXPORT virtual auto GetOpResult() const noexcept -> CGSS_OP_RESULT;

    [[nodiscard]] CGSS_EXPORT auto what() const noexcept -> const char * override;

protected:
    CGSS_OP_RESULT _result;
    std::string _message;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_
