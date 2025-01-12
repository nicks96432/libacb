#include <string>

#include "acb_enum.h"
#include "acb_env_ns.h"
#include "takamori/exceptions/CFormatException.h"

ACB_NS_BEGIN

CFormatException::CFormatException() noexcept: MyClass("") {}

CFormatException::CFormatException(const char *message) noexcept
    : MyBase(OpResult::FormatError, message) {}

CFormatException::CFormatException(const std::string &message) noexcept
    : MyBase(OpResult::FormatError, message) {}

CFormatException::CFormatException(const CFormatException &exception) noexcept = default;

ACB_NS_END
