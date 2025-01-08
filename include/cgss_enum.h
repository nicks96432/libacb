#ifndef CGSS_ENUM_H_
#define CGSS_ENUM_H_

#include <cstdint>
#include <type_traits>

#include "cgss_cenum.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

enum class StreamSeekOrigin : std::uint32_t {
    Begin   = CGSS_ORIGIN_BEGIN,
    Current = CGSS_ORIGIN_CURRENT,
    End     = CGSS_ORIGIN_END,
};

enum class HcaCipherType : std::uint32_t {
    NoCipher = CGSS_HCA_CIPH_NO_CIPHER,
    Static   = CGSS_HCA_CIPH_STATIC,
    WithKey  = CGSS_HCA_CIPH_WITH_KEY,
};

enum class UtfColumnType : std::uint8_t {
    U8     = CGSS_UTF_COLUMN_TYPE_U8,
    S8     = CGSS_UTF_COLUMN_TYPE_S8,
    U16    = CGSS_UTF_COLUMN_TYPE_U16,
    S16    = CGSS_UTF_COLUMN_TYPE_S16,
    U32    = CGSS_UTF_COLUMN_TYPE_U32,
    S32    = CGSS_UTF_COLUMN_TYPE_S32,
    U64    = CGSS_UTF_COLUMN_TYPE_U64,
    S64    = CGSS_UTF_COLUMN_TYPE_S64,
    R32    = CGSS_UTF_COLUMN_TYPE_R32,
    R64    = CGSS_UTF_COLUMN_TYPE_R64,
    String = CGSS_UTF_COLUMN_TYPE_STRING,
    Data   = CGSS_UTF_COLUMN_TYPE_DATA,
    Mask   = CGSS_UTF_COLUMN_TYPE_MASK,
};

enum class UtfColumnStorage : std::uint8_t {
    Zero   = CGSS_UTF_COLUMN_STORAGE_ZERO,
    Const  = CGSS_UTF_COLUMN_STORAGE_CONST,
    PerRow = CGSS_UTF_COLUMN_STORAGE_PER_ROW,
    Const2 = CGSS_UTF_COLUMN_STORAGE_CONST2,
    Mask   = CGSS_UTF_COLUMN_STORAGE_MASK,
};

enum class AcbWaveformEncodeType : std::uint8_t {
    Adx         = CGSS_ACB_WAVEFORM_ADX,
    Hca         = CGSS_ACB_WAVEFORM_HCA,
    Hca2        = CGSS_ACB_WAVEFORM_HCA2,
    Vag         = CGSS_ACB_WAVEFORM_VAG,
    Atrac3      = CGSS_ACB_WAVEFORM_ATRAC3,
    BcWav       = CGSS_ACB_WAVEFORM_BCWAV,
    NintendoDsp = CGSS_ACB_WAVEFORM_NINTENDO_DSP,
};

enum class OpResult : std::int32_t {
    OK               = CGSS_OP_OK,
    GenericFault     = CGSS_OP_GENERIC_FAULT,
    BufferTooSmall   = CGSS_OP_BUFFER_TOO_SMALL,
    NotImplemented   = CGSS_OP_NOT_IMPLEMENTED,
    InvalidOperation = CGSS_OP_INVALID_OPERATION,
    InvalidArgument  = CGSS_OP_INVALID_ARGUMENT,
    FormatError      = CGSS_OP_FORMAT_ERROR,
    ChecksumError    = CGSS_OP_CHECKSUM_ERROR,
    DecodeFailed     = CGSS_OP_DECODE_FAILED,
    InvalidHandle    = CGSS_OP_INVALID_HANDLE,
};

enum class FileMode : std::uint32_t {
    None         = CGSS_FILE_MODE_NONE,
    Append       = CGSS_FILE_MODE_APPEND,
    Create       = CGSS_FILE_MODE_CREATE,
    CreateNew    = CGSS_FILE_MODE_CREATE_NEW,
    OpenExisting = CGSS_FILE_MODE_OPEN_EXISTING,
    OpenOrCreate = CGSS_FILE_MODE_OPEN_OR_CREATE,
};

enum class FileAccess : std::uint32_t {
    None      = CGSS_FILE_ACCESS_NONE,
    Read      = CGSS_FILE_ACCESS_READ,
    Write     = CGSS_FILE_ACCESS_WRITE,
    ReadWrite = CGSS_FILE_ACCESS_READ_WRITE,
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

CGSS_NS_END

#endif // CGSS_ENUM_H_
