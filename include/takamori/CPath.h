#ifndef CGSS_TAKAMORI_CPATH_H_
#define CGSS_TAKAMORI_CPATH_H_

#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CPath final {

    PURE_STATIC(CPath);

public:
    static auto Combine(const std::string &basePath, const std::string &path1) -> std::string;

    static auto GetExtension(const std::string &path) -> std::string;

    static auto GetFileName(const std::string &path) -> std::string;

    static auto GetDirectoryName(const std::string &path) -> std::string;

    static auto GetFileBaseName(const std::string &path) -> std::string;

private:
    static auto FindLastSlash(const std::string &path) -> std::string::size_type;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_CPATH_H_
