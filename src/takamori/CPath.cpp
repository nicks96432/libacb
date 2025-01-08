#include <algorithm>
#include <string>

#include "cgss_env_ns.h"
#include "takamori/CPath.h"

CGSS_NS_BEGIN

auto CPath::Combine(const std::string &basePath, const std::string &path1) -> std::string {
    if (basePath.empty()) {
        return path1;
    }

    if (path1.empty()) {
        return basePath;
    }

    const auto lastBaseChar    = basePath[basePath.size() - 1];
    const auto firstAppendChar = path1[0];

    if ((lastBaseChar != '/' && lastBaseChar != '\\') &&
        (firstAppendChar != '/' && firstAppendChar != '\\')) {
        return basePath + "/" + path1;
    } else {
        return basePath + path1;
    }
}

auto CPath::GetExtension(const std::string &path) -> std::string {
    const auto dotPos = path.rfind('.');

    if (dotPos == std::string::npos) {
        return "";
    }

    if (dotPos == path.size() - 1) {
        return "";
    }

    const auto slashPos = FindLastSlash(path);

    if (slashPos != std::string::npos) {
        if (dotPos < slashPos) {
            return "";
        }
    }

    // Including the dot
    return path.substr(dotPos);
}

auto CPath::GetFileName(const std::string &path) -> std::string {
    const auto slashPos = FindLastSlash(path);

    if (slashPos == std::string::npos) {
        return path;
    }

    if (slashPos == path.size() - 1) {
        return "";
    }

    return path.substr(slashPos + 1u);
}

auto CPath::GetDirectoryName(const std::string &path) -> std::string {
    const auto slashPos = FindLastSlash(path);

    if (slashPos == std::string::npos) {
        return "";
    }

    return path.substr(0, slashPos);
}

auto CPath::GetFileBaseName(const std::string &path) -> std::string {
    const auto fileName = GetFileName(path);

    const auto dotPos = fileName.rfind('.');

    if (dotPos == std::string::npos) {
        return fileName;
    }

    return fileName.substr(0, dotPos);
}

auto CPath::FindLastSlash(const std::string &path) -> std::string::size_type {
    const auto forward  = path.rfind('/');
    const auto backward = path.rfind('\\');

    if (forward == std::string::npos) {
        return backward;
    }

    if (backward == std::string::npos) {
        return forward;
    }

    return std::max(forward, backward);
}

CGSS_NS_END
