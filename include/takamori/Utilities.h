#ifndef CGSS_TAKAMORI_UTILITIES_H_
#define CGSS_TAKAMORI_UTILITIES_H_

#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CGSS_EXPORT Utilities {

public:
    using HLIB = void *;

    static auto LoadDynamicLibrary(const char *lpstrModuleName) -> HLIB;

    static auto GetFunctionAddress(HLIB hModule, const char *lpstrFuncName) -> void *;

    static auto FreeDynamicLibrary(HLIB hModule) -> bool_t;

    PURE_STATIC(Utilities);
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_UTILITIES_H_
