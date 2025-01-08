#ifndef CGSS_TAKAMORI_CFILESYSTEM_H_
#define CGSS_TAKAMORI_CFILESYSTEM_H_

#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CFileSystem final {

    PURE_STATIC(CFileSystem);

public:
    static auto FileExists(const std::string &path) -> bool_t;

    static auto DirectoryExists(const std::string &path) -> bool_t;

    static auto FileExists(const char *path) -> bool_t;

    static auto DirectoryExists(const char *path) -> bool_t;

    static auto MkDir(const std::string &path) -> bool_t;

    static auto MkDir(const char *path) -> bool_t;

    static auto RmFile(const std::string &path) -> bool_t;

    static auto RmFile(const char *path) -> bool_t;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_CFILESYSTEM_H_
