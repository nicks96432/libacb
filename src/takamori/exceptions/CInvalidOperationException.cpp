#include <string>

#include "cgss_env_ns.h"
#include "takamori/exceptions/CInvalidOperationException.h"

CGSS_NS_BEGIN

CInvalidOperationException::CInvalidOperationException() noexcept: MyClass("") {}

CInvalidOperationException::CInvalidOperationException(const char *message) noexcept
    : MyBase(CGSS_OP_INVALID_OPERATION, message) {}

CInvalidOperationException::CInvalidOperationException(const std::string &message) noexcept
    : MyBase(CGSS_OP_INVALID_OPERATION, message) {}

CInvalidOperationException::CInvalidOperationException(const CInvalidOperationException &exception
) noexcept = default;

CGSS_NS_END
