#include <string>

#include "acb_enum.h"
#include "acb_env_ns.h"
#include "takamori/exceptions/CException.h"

ACB_NS_BEGIN

CException::CException() noexcept: MyClass("") {}

// Intended
CException::CException(const CException &) noexcept: MyBase(), _result(OpResult::GenericFault) {}

CException::CException(OpResult result) noexcept: MyClass(result, "") {}

CException::CException(const char *message) noexcept: MyClass(std::string(message)) {}

CException::CException(const std::string &message) noexcept
    : MyClass(OpResult::GenericFault, message) {}

CException::CException(OpResult result, std::string message) noexcept
    : MyBase(), _result(result), _message(std::move(message)) {}

auto CException::operator=(const CException &exception) noexcept -> CException & {
    _result  = exception._result;
    _message = exception._message;

    return *this;
}

auto CException::GetExceptionMessage() const noexcept -> const std::string & {
    return _message;
}

auto CException::GetOpResult() const noexcept -> OpResult {
    return _result;
}

auto CException::what() const noexcept -> const char * {
    return _message.c_str();
}

ACB_NS_END
