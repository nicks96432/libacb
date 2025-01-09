#include "acb_env_platform.h"

#if defined(__ACB_OS_WINDOWS__)

#include <windows.h>

#elif defined(__ACB_OS_UNIX__)

#include <dlfcn.h>

#endif

#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/Utilities.h"

ACB_NS_BEGIN

auto Utilities::LoadDynamicLibrary(const char *lpstrModuleName) -> acb::Utilities::HLIB {
#if defined(__ACB_OS_WINDOWS__)
    return LoadLibrary(lpstrModuleName);
#elif defined(__ACB_OS_UNIX__)
    return ::dlopen(lpstrModuleName, RTLD_LAZY);
#endif
}

auto Utilities::GetFunctionAddress(acb::Utilities::HLIB hModule, const char *lpstrFuncName)
    -> void * {
#if defined(__ACB_OS_WINDOWS__)
    return (void *)GetProcAddress((HMODULE)hModule, lpstrFuncName);
#elif defined(__ACB_OS_UNIX__)
    return ::dlsym(hModule, lpstrFuncName);
#endif
}

auto Utilities::FreeDynamicLibrary(acb::Utilities::HLIB hModule) -> bool_t {
#if defined(__ACB_OS_WINDOWS__)
    return static_cast<bool_t>(FreeLibrary((HMODULE)hModule));
#elif defined(__ACB_OS_UNIX__)
    return static_cast<bool_t>(::dlclose(hModule));
#endif
}

ACB_NS_END
