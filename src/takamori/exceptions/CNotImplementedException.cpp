#include <string>

#include "acb_env_ns.h"
#include "takamori/exceptions/CNotImplementedException.h"

ACB_NS_BEGIN

CNotImplementedException::CNotImplementedException() noexcept: MyClass("") {}

CNotImplementedException::CNotImplementedException(const char *message) noexcept
    : MyBase(ACB_OP_NOT_IMPLEMENTED, message) {}

CNotImplementedException::CNotImplementedException(const std::string &message) noexcept
    : MyBase(ACB_OP_NOT_IMPLEMENTED, message) {}

CNotImplementedException::CNotImplementedException(const acb::CNotImplementedException &exception
) noexcept = default;

ACB_NS_END
