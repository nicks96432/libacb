#ifndef ACB_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_
#define ACB_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_

#include <exception>
#include <string>

#include "acb_cenum.h"
#include "acb_env.h"
#include "acb_env_ns.h"
#include "acb_env_platform.h"

ACB_NS_BEGIN

class CException: public std::exception {

    __extends(std::exception, CException);

public:
    ACB_EXPORT CException() noexcept;

    ACB_EXPORT CException(const CException &) noexcept;

    ACB_EXPORT auto operator=(const CException &) noexcept -> CException &;

    ACB_EXPORT explicit CException(ACB_OP_RESULT result) noexcept;

    ACB_EXPORT explicit CException(const char *message) noexcept;

    ACB_EXPORT explicit CException(const std::string &message) noexcept;

    ACB_EXPORT CException(ACB_OP_RESULT result, std::string message) noexcept;

    ACB_EXPORT ~CException() noexcept override = default;

    [[nodiscard]] ACB_EXPORT virtual auto
    GetExceptionMessage() const noexcept -> const std::string &;

    [[nodiscard]] ACB_EXPORT virtual auto GetOpResult() const noexcept -> ACB_OP_RESULT;

    [[nodiscard]] ACB_EXPORT auto what() const noexcept -> const char * override;

protected:
    ACB_OP_RESULT _result;
    std::string _message;
};

ACB_NS_END

#endif // ACB_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_
