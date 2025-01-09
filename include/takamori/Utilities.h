#ifndef ACB_TAKAMORI_UTILITIES_H_
#define ACB_TAKAMORI_UTILITIES_H_

#include "acb_env.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

class ACB_EXPORT Utilities {

public:
    using HLIB = void *;

    static auto LoadDynamicLibrary(const char *lpstrModuleName) -> HLIB;

    static auto GetFunctionAddress(HLIB hModule, const char *lpstrFuncName) -> void *;

    static auto FreeDynamicLibrary(HLIB hModule) -> bool_t;

    PURE_STATIC(Utilities);
};

ACB_NS_END

#endif // ACB_TAKAMORI_UTILITIES_H_
