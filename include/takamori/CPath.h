#ifndef ACB_TAKAMORI_CPATH_H_
#define ACB_TAKAMORI_CPATH_H_

#include <string>

#include "acb_env.h"
#include "acb_env_ns.h"

ACB_NS_BEGIN

class ACB_EXPORT CPath final {

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

ACB_NS_END

#endif // ACB_TAKAMORI_CPATH_H_
