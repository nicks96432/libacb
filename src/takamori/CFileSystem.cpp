#include <filesystem>
#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "takamori/CFileSystem.h"

CGSS_NS_BEGIN

auto CFileSystem::FileExists(const std::string &path) -> bool_t {
    return std::filesystem::is_regular_file(path);
}

auto CFileSystem::DirectoryExists(const std::string &path) -> bool_t {
    return std::filesystem::is_directory(path);
}

auto CFileSystem::MkDir(const std::string &path) -> bool_t {
    return std::filesystem::create_directories(path);
}

auto CFileSystem::RmFile(const std::string &path) -> bool_t {
    return std::filesystem::remove(path);
}

CGSS_NS_END
