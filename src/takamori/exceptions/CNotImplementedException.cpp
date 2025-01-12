#include <string>

#include "acb_enum.h"
#include "acb_env_ns.h"
#include "takamori/exceptions/CNotImplementedException.h"

ACB_NS_BEGIN

CNotImplementedException::CNotImplementedException() noexcept: MyClass("") {}

CNotImplementedException::CNotImplementedException(const char *message) noexcept
    : MyBase(OpResult::NotImplemented, message) {}

CNotImplementedException::CNotImplementedException(const std::string &message) noexcept
    : MyBase(OpResult::NotImplemented, message) {}

CNotImplementedException::CNotImplementedException(const acb::CNotImplementedException &exception
) noexcept = default;

ACB_NS_END
