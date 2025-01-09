#ifndef ACB_CENUM_H_
#define ACB_CENUM_H_

enum ACB_ACB_WAVEFORM_ENCODE_TYPE {
    ACB_ACB_WAVEFORM_ADX          = 0,
    ACB_ACB_WAVEFORM_HCA          = 2,
    ACB_ACB_WAVEFORM_HCA2         = 6,
    ACB_ACB_WAVEFORM_VAG          = 7,
    ACB_ACB_WAVEFORM_ATRAC3       = 8,
    ACB_ACB_WAVEFORM_BCWAV        = 9,
    ACB_ACB_WAVEFORM_NINTENDO_DSP = 13,
};

enum ACB_FILE_ACCESS {
    ACB_FILE_ACCESS_NONE         = 0,
    ACB_FILE_ACCESS_READ         = 1,
    ACB_FILE_ACCESS_WRITE        = 2,
    ACB_FILE_ACCESS_READ_WRITE   = 3,
    _ACB_FILE_ACCESS_FORCE_DWORD = 0x7fffffff,
};

enum ACB_FILE_MODE {
    ACB_FILE_MODE_NONE           = 0,
    ACB_FILE_MODE_APPEND         = 1,
    ACB_FILE_MODE_CREATE         = 2,
    ACB_FILE_MODE_CREATE_NEW     = 3,
    ACB_FILE_MODE_OPEN_EXISTING  = 4,
    ACB_FILE_MODE_OPEN_OR_CREATE = 5,
    _ACB_FILE_MODE_FORCE_DWORD   = 0x7fffffff,
};

enum ACB_HCA_CIPHER_TYPE {
    ACB_HCA_CIPH_NO_CIPHER    = 0,
    ACB_HCA_CIPH_STATIC       = 1,
    ACB_HCA_CIPH_WITH_KEY     = 56,
    _ACB_HCA_CIPH_FORCE_DWORD = 0x7fffffff,
};


enum ACB_OP_RESULT {
    ACB_OP_OK                = 0,
    ACB_OP_GENERIC_FAULT     = -1,
    ACB_OP_BUFFER_TOO_SMALL  = -2,
    ACB_OP_NOT_IMPLEMENTED   = -3,
    ACB_OP_INVALID_OPERATION = -4,
    ACB_OP_INVALID_ARGUMENT  = -5,
    ACB_OP_FORMAT_ERROR      = -6,
    ACB_OP_CHECKSUM_ERROR    = -7,
    ACB_OP_DECODE_FAILED     = -8,
    ACB_OP_INVALID_HANDLE    = -9,
    _ACB_OP_FORCE_DWORD      = 0x7fffffff,
};

#define ACB_OP_SUCCEEDED(x) ((ACB_OP_RESULT)(x) >= 0)
#define ACB_OP_FAILED(x)    ((ACB_OP_RESULT)(x) < 0)

enum ACB_STREAM_SEEK_ORIGIN {
    ACB_ORIGIN_BEGIN        = 0,
    ACB_ORIGIN_CURRENT      = 1,
    ACB_ORIGIN_END          = 2,
    _ACB_ORIGIN_FORCE_DWORD = 0x7fffffff,
};

enum ACB_UTF_COLUMN_STORAGE {
    ACB_UTF_COLUMN_STORAGE_ZERO    = 0x10,
    ACB_UTF_COLUMN_STORAGE_CONST   = 0x30,
    ACB_UTF_COLUMN_STORAGE_PER_ROW = 0x50,
    ACB_UTF_COLUMN_STORAGE_CONST2  = 0x70,
    ACB_UTF_COLUMN_STORAGE_MASK    = 0xf0,
};

enum ACB_UTF_COLUMN_TYPE {
    ACB_UTF_COLUMN_TYPE_U8     = 0,
    ACB_UTF_COLUMN_TYPE_S8     = 1,
    ACB_UTF_COLUMN_TYPE_U16    = 2,
    ACB_UTF_COLUMN_TYPE_S16    = 3,
    ACB_UTF_COLUMN_TYPE_U32    = 4,
    ACB_UTF_COLUMN_TYPE_S32    = 5,
    ACB_UTF_COLUMN_TYPE_U64    = 6,
    ACB_UTF_COLUMN_TYPE_S64    = 7,
    ACB_UTF_COLUMN_TYPE_R32    = 8,
    ACB_UTF_COLUMN_TYPE_R64    = 9,
    ACB_UTF_COLUMN_TYPE_STRING = 10,
    ACB_UTF_COLUMN_TYPE_DATA   = 11,
    ACB_UTF_COLUMN_TYPE_MASK   = 0x0f,
};

#endif // ACB_CENUM_H_
