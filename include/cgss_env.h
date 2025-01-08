#ifndef CGSS_ENV_H_
#define CGSS_ENV_H_

#include "cgss_env_platform.h"

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW__)
#ifdef __GNUC__
#define CGSS_EXPORT __attribute__((dllexport))
#else
#define CGSS_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#define CGSS_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define CGSS_EXPORT    __attribute__((visibility("default")))
#define CGSS_DLL_LOCAL __attribute__((visibility("hidden")))
#else
#define CGSS_EXPORT
#define CGSS_DLL_LOCAL
#endif
#endif

#ifdef _MSC_VER
#ifndef __func__
#define __func__ __FUNCTION__
#endif
#endif

#ifndef STDCALL

#ifdef __CGSS_OS_WINDOWS__
#ifndef STDCALL
#define STDCALL __stdcall
#endif // STDCALL
#else
#define STDCALL
#endif // __CGSS_OS_WINDOWS__

#endif // STDCALL

#include <cstdint>
#include <cstdlib>
#include <cstring>

using bool_t = std::uint32_t;

#ifndef __CGSS_OS_WINDOWS__
using LPCSTR = const char *;
#endif
#ifndef TRUE
#define TRUE ((bool_t)1)
#endif
#ifndef FALSE
#define FALSE ((bool_t)0)
#endif

#ifndef PURE
#define PURE = 0
#endif

#ifndef _OUT_
#define _OUT_
#endif
#ifndef _IN_
#define _IN_
#endif
#ifndef _REF_
#define _REF_
#endif

#ifdef __CGSS_OS_WINDOWS__

#ifndef _MBCS
#define _MBCS
#endif

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

// "#ifndef" for warnings in Cygwin/MinGW
#ifndef NOMINMAX
#define NOMINMAX
#endif

#endif // __CGSS_OS_WINDOWS__

#define PURE_STATIC(className)                               \
public:                                                      \
    className()                                    = delete; \
    className(const className &)                   = delete; \
    className(className &&)                        = delete; \
    auto operator=(const className &)->className & = delete; \
    auto operator=(className &&)->className      & = delete

#define __root_class(cls) \
private:                  \
    typedef cls MyClass

#define __extends(parent_cls, child_cls) \
private:                                 \
    typedef parent_cls MyBase;           \
    typedef child_cls MyClass

#endif // CGSS_ENV_H_
