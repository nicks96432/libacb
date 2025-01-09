#include <filesystem>
#include <string>

#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/CFileSystem.h"

ACB_NS_BEGIN

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

ACB_NS_END
