#include <string>

#include "cgss_cenum.h"
#include "cgss_env_ns.h"
#include "takamori/exceptions/CArgumentException.h"

CGSS_NS_BEGIN

CArgumentException::CArgumentException() noexcept: MyClass("") {}

CArgumentException::CArgumentException(const char *message) noexcept
    : MyBase(CGSS_OP_INVALID_ARGUMENT, message) {}

CArgumentException::CArgumentException(const std::string &message) noexcept
    : MyBase(CGSS_OP_INVALID_ARGUMENT, message) {}

CArgumentException::CArgumentException(const CArgumentException &exception) noexcept = default;

CGSS_NS_END
