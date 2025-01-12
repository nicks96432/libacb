#ifndef ACB_ENUM_H_
#define ACB_ENUM_H_

#include <cstdint>
#include <type_traits>
#include <utility>

#include "acb_env_ns.h"

ACB_NS_BEGIN

enum class StreamSeekOrigin : std::uint8_t {
    Begin   = 0,
    Current = 1,
    End     = 2,
};

enum class HcaCipherType : std::uint32_t {
    NoCipher = 0,
    Static   = 1,
    WithKey  = 56,
};

enum class UtfColumnType : std::uint8_t {
    U8     = 0,
    S8     = 1,
    U16    = 2,
    S16    = 3,
    U32    = 4,
    S32    = 5,
    U64    = 6,
    S64    = 7,
    R32    = 8,
    R64    = 9,
    String = 10,
    Data   = 11,
    Mask   = 0x0f,
};

enum class UtfColumnStorage : std::uint8_t {
    Zero   = 0x10,
    Const  = 0x30,
    PerRow = 0x50,
    Const2 = 0x70,
    Mask   = 0xf0,
};

enum class AcbWaveformEncodeType : std::uint8_t {
    Adx         = 0,
    Hca         = 2,
    Hca2        = 6,
    Vag         = 7,
    Atrac3      = 8,
    BcWav       = 9,
    NintendoDSP = 13,
};

enum class OpResult : std::int8_t {
    OK               = 0,
    GenericFault     = -1,
    BufferTooSmall   = -2,
    NotImplemented   = -3,
    InvalidOperation = -4,
    InvalidArgument  = -5,
    FormatError      = -6,
    ChecksumError    = -7,
    DecodeFailed     = -8,
    InvalidHandle    = -9,
};

enum class FileMode : std::uint8_t {
    None         = 0,
    Append       = 1,
    Create       = 2,
    CreateNew    = 3,
    OpenExisting = 4,
    OpenOrCreate = 5,
};

enum class FileAccess : std::uint8_t {
    None      = 0,
    Read      = 1,
    Write     = 2,
    ReadWrite = 3,
};

// NOLINTBEGIN(bugprone-macro-parentheses)

#define DEFINE_ENUM_CLS_BINARY_OP(cls, op)                            \
    inline auto operator op(cls l, cls r)->cls {                      \
        return (cls)(std::to_underlying(l) op std::to_underlying(r)); \
    }

#define DEFINE_ENUM_CLS_UNARY_OP(cls, op)       \
    inline operator op(cls v)->cls {            \
        return (cls)(op std::to_underlying(v)); \
    }

// NOLINTEND(bugprone-macro-parentheses)

DEFINE_ENUM_CLS_BINARY_OP(FileMode, |)

DEFINE_ENUM_CLS_BINARY_OP(FileMode, &)

DEFINE_ENUM_CLS_BINARY_OP(FileAccess, |)

DEFINE_ENUM_CLS_BINARY_OP(FileAccess, &)

ACB_NS_END

#endif // ACB_ENUM_H_
