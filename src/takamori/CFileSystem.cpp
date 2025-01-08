
#ifdef __CGSS_OS_WINDOWS__

#include <shlobj.h>

#else

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#endif

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "takamori/CFileSystem.h"

CGSS_NS_BEGIN

auto CFileSystem::FileExists(const std::string &path) -> bool_t {
    return FileExists(path.c_str());
}

auto CFileSystem::DirectoryExists(const std::string &path) -> bool_t {
    return DirectoryExists(path.c_str());
}

auto CFileSystem::FileExists(const char *path) -> bool_t {
    auto fp = std::fopen(path, "r");

    if (!fp) {
        return FALSE;
    }

    std::fclose(fp);

    return TRUE;
}

auto CFileSystem::MkDir(const std::string &path) -> bool_t {
    return MkDir(path.c_str());
}

auto CFileSystem::RmFile(const std::string &path) -> bool_t {
    return RmFile(path.c_str());
}

auto CFileSystem::RmFile(const char *path) -> bool_t {
    const auto success = std::remove(path) == 0;
    return static_cast<bool_t>(success);
}

#ifdef __CGSS_OS_WINDOWS__

bool_t CFileSystem::DirectoryExists(const char *path) {
    const auto type = GetFileAttributes(path);

    if (type == INVALID_FILE_ATTRIBUTES) {
        // Cannot open the entry
        return FALSE;
    } else if (type == FILE_ATTRIBUTE_DIRECTORY) {
        // Everything is OK
        return TRUE;
    } else {
        // It is not a directory
        return FALSE;
    }
}

// http://blog.nuclex-games.com/2012/06/how-to-create-directories-recursively-with-win32/
bool_t CFileSystem::MkDir(const char *path) {
    static const std::string separators("\\/");
    const std::string directory(path);

    DWORD fileAttributes = ::GetFileAttributes(directory.c_str());

    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {

        std::size_t slashIndex = directory.find_last_of(separators);

        if (slashIndex != std::wstring::npos) {
            MkDir(directory.substr(0, slashIndex));
        }

        BOOL result = ::CreateDirectory(directory.c_str(), nullptr);

        if (result == FALSE) {
            return FALSE;
        }
    } else {
        bool isDirectoryOrJunction = ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
                                     ((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

        if (!isDirectoryOrJunction) {
            // There is a file with the same name.
            return FALSE;
        }
    }

    return TRUE;
}

#else

auto CFileSystem::DirectoryExists(const char *path) -> bool_t {
    struct stat info {};

    if (::stat(path, &info) != 0) {
        // Cannot open the entry
        return FALSE;
    } else if (info.st_mode & S_IFDIR) {
        // Everything is OK
        return TRUE;
    } else {
        // It is not a directory
        return FALSE;
    }
}

// http://nion.modprobe.de/blog/archives/357-Recursive-directory-creation.html
auto CFileSystem::MkDir(const char *path) -> bool_t {
    if (path == nullptr) {
        return FALSE;
    }

    if (std::strlen(path) == 0) {
        return FALSE;
    }

    char tmp[256];
    char *p = nullptr;
    std::size_t len;

    std::strncpy(tmp, path, sizeof(tmp));

    len = std::strlen(tmp);

    if (tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }

    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            ::mkdir(tmp, S_IRWXU);
            *p = '/';
        }

    ::mkdir(tmp, S_IRWXU);

    return TRUE;
}

#endif

CGSS_NS_END