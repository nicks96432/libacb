#include "acb_env_platform.h"

#ifdef ACB_OS_WINDOWS

#include <windows.h>

#elifdef ACB_OS_UNIX

#include <dlfcn.h>

#endif

#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/Utilities.h"

ACB_NS_BEGIN

auto Utilities::LoadDynamicLibrary(const char *lpstrModuleName) -> acb::Utilities::HLIB {
#ifdef ACB_OS_WINDOWS
    return LoadLibrary(lpstrModuleName);
#elifdef ACB_OS_UNIX
    return ::dlopen(lpstrModuleName, RTLD_LAZY);
#endif
}

auto Utilities::GetFunctionAddress(acb::Utilities::HLIB hModule, const char *lpstrFuncName)
    -> void * {
#ifdef ACB_OS_WINDOWS
    return (void *)GetProcAddress((HMODULE)hModule, lpstrFuncName);
#elifdef ACB_OS_UNIX
    return ::dlsym(hModule, lpstrFuncName);
#endif
}

auto Utilities::FreeDynamicLibrary(acb::Utilities::HLIB hModule) -> bool_t {
#ifdef ACB_OS_WINDOWS
    return static_cast<bool_t>(FreeLibrary((HMODULE)hModule));
#elifdef ACB_OS_UNIX
    return static_cast<bool_t>(::dlclose(hModule));
#endif
}

ACB_NS_END
