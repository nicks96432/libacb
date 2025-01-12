#include <string>

#include "acb_enum.h"
#include "acb_env_ns.h"
#include "takamori/exceptions/CArgumentException.h"

ACB_NS_BEGIN

CArgumentException::CArgumentException() noexcept: MyClass{""} {}

CArgumentException::CArgumentException(const char *message) noexcept
    : MyBase{OpResult::InvalidArgument, message} {}

CArgumentException::CArgumentException(const std::string &message) noexcept
    : MyBase{OpResult::InvalidArgument, message} {}

CArgumentException::CArgumentException(const CArgumentException &exception) noexcept = default;

ACB_NS_END
