#ifndef ACB_ENUM_H_
#define ACB_ENUM_H_

#include <cstdint>
#include <type_traits>

#include "acb_cenum.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

enum class StreamSeekOrigin : std::uint32_t {
    Begin   = ACB_ORIGIN_BEGIN,
    Current = ACB_ORIGIN_CURRENT,
    End     = ACB_ORIGIN_END,
};

enum class HcaCipherType : std::uint32_t {
    NoCipher = ACB_HCA_CIPH_NO_CIPHER,
    Static   = ACB_HCA_CIPH_STATIC,
    WithKey  = ACB_HCA_CIPH_WITH_KEY,
};

enum class UtfColumnType : std::uint8_t {
    U8     = ACB_UTF_COLUMN_TYPE_U8,
    S8     = ACB_UTF_COLUMN_TYPE_S8,
    U16    = ACB_UTF_COLUMN_TYPE_U16,
    S16    = ACB_UTF_COLUMN_TYPE_S16,
    U32    = ACB_UTF_COLUMN_TYPE_U32,
    S32    = ACB_UTF_COLUMN_TYPE_S32,
    U64    = ACB_UTF_COLUMN_TYPE_U64,
    S64    = ACB_UTF_COLUMN_TYPE_S64,
    R32    = ACB_UTF_COLUMN_TYPE_R32,
    R64    = ACB_UTF_COLUMN_TYPE_R64,
    String = ACB_UTF_COLUMN_TYPE_STRING,
    Data   = ACB_UTF_COLUMN_TYPE_DATA,
    Mask   = ACB_UTF_COLUMN_TYPE_MASK,
};

enum class UtfColumnStorage : std::uint8_t {
    Zero   = ACB_UTF_COLUMN_STORAGE_ZERO,
    Const  = ACB_UTF_COLUMN_STORAGE_CONST,
    PerRow = ACB_UTF_COLUMN_STORAGE_PER_ROW,
    Const2 = ACB_UTF_COLUMN_STORAGE_CONST2,
    Mask   = ACB_UTF_COLUMN_STORAGE_MASK,
};

enum class AcbWaveformEncodeType : std::uint8_t {
    Adx         = ACB_ACB_WAVEFORM_ADX,
    Hca         = ACB_ACB_WAVEFORM_HCA,
    Hca2        = ACB_ACB_WAVEFORM_HCA2,
    Vag         = ACB_ACB_WAVEFORM_VAG,
    Atrac3      = ACB_ACB_WAVEFORM_ATRAC3,
    BcWav       = ACB_ACB_WAVEFORM_BCWAV,
    NintendoDsp = ACB_ACB_WAVEFORM_NINTENDO_DSP,
};

enum class OpResult : std::int32_t {
    OK               = ACB_OP_OK,
    GenericFault     = ACB_OP_GENERIC_FAULT,
    BufferTooSmall   = ACB_OP_BUFFER_TOO_SMALL,
    NotImplemented   = ACB_OP_NOT_IMPLEMENTED,
    InvalidOperation = ACB_OP_INVALID_OPERATION,
    InvalidArgument  = ACB_OP_INVALID_ARGUMENT,
    FormatError      = ACB_OP_FORMAT_ERROR,
    ChecksumError    = ACB_OP_CHECKSUM_ERROR,
    DecodeFailed     = ACB_OP_DECODE_FAILED,
    InvalidHandle    = ACB_OP_INVALID_HANDLE,
};

enum class FileMode : std::uint32_t {
    None         = ACB_FILE_MODE_NONE,
    Append       = ACB_FILE_MODE_APPEND,
    Create       = ACB_FILE_MODE_CREATE,
    CreateNew    = ACB_FILE_MODE_CREATE_NEW,
    OpenExisting = ACB_FILE_MODE_OPEN_EXISTING,
    OpenOrCreate = ACB_FILE_MODE_OPEN_OR_CREATE,
};

enum class FileAccess : std::uint32_t {
    None      = ACB_FILE_ACCESS_NONE,
    Read      = ACB_FILE_ACCESS_READ,
    Write     = ACB_FILE_ACCESS_WRITE,
    ReadWrite = ACB_FILE_ACCESS_READ_WRITE,
};

#define DEFINE_ENUM_CLS_BINARY_OP(cls, op)                                \
    inline auto operator op(cls l, cls r)->cls {                          \
        return (cls)(static_cast<std::underlying_type_t<cls>>(l)          \
                         op static_cast<std::underlying_type_t<cls>>(r)); \
    }

#define DEFINE_ENUM_CLS_UNARY_OP(cls, op)                             \
    inline operator op(cls v)->cls {                                  \
        return (cls)(op static_cast<std::underlying_type_t<cls>>(v)); \
    }

DEFINE_ENUM_CLS_BINARY_OP(FileMode, |)

DEFINE_ENUM_CLS_BINARY_OP(FileMode, &)

DEFINE_ENUM_CLS_BINARY_OP(FileAccess, |)

DEFINE_ENUM_CLS_BINARY_OP(FileAccess, &)

ACB_NS_END

#endif // ACB_ENUM_H_
