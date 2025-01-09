#include <string>

#include "acb_cenum.h"
#include "acb_env_ns.h"
#include "takamori/exceptions/CArgumentException.h"

ACB_NS_BEGIN

CArgumentException::CArgumentException() noexcept: MyClass("") {}

CArgumentException::CArgumentException(const char *message) noexcept
    : MyBase(ACB_OP_INVALID_ARGUMENT, message) {}

CArgumentException::CArgumentException(const std::string &message) noexcept
    : MyBase(ACB_OP_INVALID_ARGUMENT, message) {}

CArgumentException::CArgumentException(const CArgumentException &exception) noexcept = default;

ACB_NS_END
