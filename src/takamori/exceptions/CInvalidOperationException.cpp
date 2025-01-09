#include <string>

#include "acb_env_ns.h"
#include "takamori/exceptions/CInvalidOperationException.h"

ACB_NS_BEGIN

CInvalidOperationException::CInvalidOperationException() noexcept: MyClass("") {}

CInvalidOperationException::CInvalidOperationException(const char *message) noexcept
    : MyBase(ACB_OP_INVALID_OPERATION, message) {}

CInvalidOperationException::CInvalidOperationException(const std::string &message) noexcept
    : MyBase(ACB_OP_INVALID_OPERATION, message) {}

CInvalidOperationException::CInvalidOperationException(const CInvalidOperationException &exception
) noexcept = default;

ACB_NS_END
