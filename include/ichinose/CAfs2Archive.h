#ifndef CGSS_ICHINOSE_CAFS2ARCHIVE_H_
#define CGSS_ICHINOSE_CAFS2ARCHIVE_H_

#include <cstdint>
#include <map>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CAfs2Archive final {

    __root_class(CAfs2Archive);

public:
    CAfs2Archive(IStream *stream, std::uint64_t offset, const char *fileName, bool_t disposeStream);

    ~CAfs2Archive();

    static auto IsAfs2Archive(IStream *stream, std::uint64_t offset) -> bool_t;

    auto GetFiles() const -> const std::map<std::uint32_t, AFS2_FILE_RECORD> &;

    auto GetStream() const -> IStream *;

    auto GetByteAlignment() const -> std::uint32_t;

    auto GetVersion() const -> std::uint32_t;

    auto GetHcaKeyModifier() const -> std::uint16_t;

    auto GetFileName() const -> const char *;

private:
    void Initialize();

    IStream *_stream;
    std::uint64_t _streamOffset;
    char *_fileName;
    bool_t _disposeStream;

    std::map<std::uint32_t, AFS2_FILE_RECORD> _files;

    std::uint32_t _byteAlignment;
    std::uint16_t _hcaKeyModifier;
    std::uint32_t _version;
};

CGSS_NS_END

#endif // CGSS_ICHINOSE_CAFS2ARCHIVE_H_
