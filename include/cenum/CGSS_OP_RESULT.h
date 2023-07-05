#pragma once

typedef enum _CGSS_OP_RESULT {
    CGSS_OP_OK                = 0,
    CGSS_OP_GENERIC_FAULT     = -1,
    CGSS_OP_BUFFER_TOO_SMALL  = -2,
    CGSS_OP_NOT_IMPLEMENTED   = -3,
    CGSS_OP_INVALID_OPERATION = -4,
    CGSS_OP_INVALID_ARGUMENT  = -5,
    CGSS_OP_FORMAT_ERROR      = -6,
    CGSS_OP_CHECKSUM_ERROR    = -7,
    CGSS_OP_DECODE_FAILED     = -8,
    CGSS_OP_INVALID_HANDLE    = -9,
    _CGSS_OP_FORCE_DWORD      = 0x7fffffff,
} CGSS_OP_RESULT;

#define CGSS_OP_SUCCEEDED(x) ((CGSS_OP_RESULT)(x) >= 0)
#define CGSS_OP_FAILED(x)    ((CGSS_OP_RESULT)(x) < 0)
