#ifndef CGSS_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_
#define CGSS_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_

#include <exception>
#include <string>

#include "cgss_cenum.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CException: public std::exception {

    __extends(std::exception, CException);

public:
    CException() noexcept;

    CException(const CException &) noexcept;

    auto operator=(const CException &) noexcept -> CException &;

    explicit CException(CGSS_OP_RESULT result) noexcept;

    explicit CException(const char *message) noexcept;

    explicit CException(const std::string &message) noexcept;

    CException(CGSS_OP_RESULT result, std::string message) noexcept;

    ~CException() noexcept override = default;

    virtual auto GetExceptionMessage() const noexcept -> const std::string &;

    virtual auto GetOpResult() const noexcept -> CGSS_OP_RESULT;

    auto what() const noexcept -> const char * override;

protected:
    CGSS_OP_RESULT _result;
    std::string _message;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_EXCEPTIONS_CEXCEPTION_H_
