#include <string>

#include "acb_env_ns.h"
#include "takamori/exceptions/CException.h"

ACB_NS_BEGIN

CException::CException() noexcept: MyClass("") {}

// Intended
CException::CException(const CException &) noexcept: MyBase(), _result(ACB_OP_GENERIC_FAULT) {}

CException::CException(ACB_OP_RESULT result) noexcept: MyClass(result, "") {}

CException::CException(const char *message) noexcept: MyClass(std::string(message)) {}

CException::CException(const std::string &message) noexcept
    : MyClass(ACB_OP_GENERIC_FAULT, message) {}

CException::CException(ACB_OP_RESULT result, std::string message) noexcept
    : MyBase(), _result(result), _message(std::move(message)) {}

auto CException::operator=(const CException &exception) noexcept -> CException & {
    _result  = exception._result;
    _message = exception._message;

    return *this;
}

auto CException::GetExceptionMessage() const noexcept -> const std::string & {
    return _message;
}

auto CException::GetOpResult() const noexcept -> ACB_OP_RESULT {
    return _result;
}

auto CException::what() const noexcept -> const char * {
    return _message.c_str();
}

ACB_NS_END
