#include "cgss_env_platform.h"

#if defined(__CGSS_OS_WINDOWS__)

#include <windows.h>

#elif defined(__CGSS_OS_UNIX__)

#include <dlfcn.h>

#endif

#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "takamori/Utilities.h"

CGSS_NS_BEGIN

auto Utilities::LoadDynamicLibrary(const char *lpstrModuleName) -> cgss::Utilities::HLIB {
#if defined(__CGSS_OS_WINDOWS__)
    return LoadLibrary(lpstrModuleName);
#elif defined(__CGSS_OS_UNIX__)
    return ::dlopen(lpstrModuleName, RTLD_LAZY);
#endif
}

auto Utilities::GetFunctionAddress(cgss::Utilities::HLIB hModule, const char *lpstrFuncName)
    -> void * {
#if defined(__CGSS_OS_WINDOWS__)
    return (void *)GetProcAddress((HMODULE)hModule, lpstrFuncName);
#elif defined(__CGSS_OS_UNIX__)
    return ::dlsym(hModule, lpstrFuncName);
#endif
}

auto Utilities::FreeDynamicLibrary(cgss::Utilities::HLIB hModule) -> bool_t {
#if defined(__CGSS_OS_WINDOWS__)
    return static_cast<bool_t>(FreeLibrary((HMODULE)hModule));
#elif defined(__CGSS_OS_UNIX__)
    return static_cast<bool_t>(::dlclose(hModule));
#endif
}

CGSS_NS_END
