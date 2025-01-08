#include <string>

#include "cgss_env_ns.h"
#include "takamori/exceptions/CNotImplementedException.h"

CGSS_NS_BEGIN

CNotImplementedException::CNotImplementedException() noexcept: MyClass("") {}

CNotImplementedException::CNotImplementedException(const char *message) noexcept
    : MyBase(CGSS_OP_NOT_IMPLEMENTED, message) {}

CNotImplementedException::CNotImplementedException(const std::string &message) noexcept
    : MyBase(CGSS_OP_NOT_IMPLEMENTED, message) {}

CNotImplementedException::CNotImplementedException(const cgss::CNotImplementedException &exception
) noexcept = default;

CGSS_NS_END
