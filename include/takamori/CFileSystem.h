#ifndef ACB_TAKAMORI_CFILESYSTEM_H_
#define ACB_TAKAMORI_CFILESYSTEM_H_

#include <string>

#include "acb_env.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

class ACB_EXPORT CFileSystem final {

    PURE_STATIC(CFileSystem);

public:
    static auto FileExists(const std::string &path) -> bool_t;

    static auto DirectoryExists(const std::string &path) -> bool_t;

    static auto MkDir(const std::string &path) -> bool_t;

    static auto RmFile(const std::string &path) -> bool_t;
};

ACB_NS_END

#endif // ACB_TAKAMORI_CFILESYSTEM_H_
