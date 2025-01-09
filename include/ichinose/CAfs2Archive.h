#ifndef CGSS_ICHINOSE_CAFS2ARCHIVE_H_
#define CGSS_ICHINOSE_CAFS2ARCHIVE_H_

#include <cstdint>
#include <map>
#include <string>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "cgss_env_platform.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

class CAfs2Archive final {

    __root_class(CAfs2Archive);

public:
    CGSS_EXPORT CAfs2Archive(
        IStream *stream, std::uint64_t offset, const std::string &fileName, bool_t disposeStream
    );

    CGSS_EXPORT ~CAfs2Archive();

    CGSS_EXPORT static auto IsAfs2Archive(IStream *stream, std::uint64_t offset) -> bool_t;

    [[nodiscard]] CGSS_EXPORT auto
    GetFiles() const -> const std::map<std::uint32_t, AFS2_FILE_RECORD> &;

    [[nodiscard]] CGSS_EXPORT auto GetStream() const -> IStream *;

    [[nodiscard]] CGSS_EXPORT auto GetByteAlignment() const -> std::uint32_t;

    [[nodiscard]] CGSS_EXPORT auto GetVersion() const -> std::uint32_t;

    [[nodiscard]] CGSS_EXPORT auto GetHcaKeyModifier() const -> std::uint16_t;

    [[nodiscard]] CGSS_EXPORT auto GetFileName() const -> const std::string &;

private:
    void Initialize();

    IStream *_stream;
    std::uint64_t _streamOffset;
    std::string _fileName;
    bool_t _disposeStream;

    std::map<std::uint32_t, AFS2_FILE_RECORD> _files;

    std::uint32_t _byteAlignment;
    std::uint16_t _hcaKeyModifier;
    std::uint32_t _version;
};

CGSS_NS_END

#endif // CGSS_ICHINOSE_CAFS2ARCHIVE_H_
