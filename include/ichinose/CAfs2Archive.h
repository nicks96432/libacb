#ifndef ACB_ICHINOSE_CAFS2ARCHIVE_H_
#define ACB_ICHINOSE_CAFS2ARCHIVE_H_

#include <cstdint>
#include <map>
#include <string>

#include "acb_cdata.h"
#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/streams/IStream.h"

ACB_NS_BEGIN

class CAfs2Archive final {

    _root_class(CAfs2Archive);

public:
    ACB_EXPORT CAfs2Archive(
        IStream *stream, std::uint64_t offset, const std::string &fileName, bool_t disposeStream
    );

    ACB_EXPORT ~CAfs2Archive();

    ACB_EXPORT static auto IsAfs2Archive(IStream *stream, std::uint64_t offset) -> bool_t;

    [[nodiscard]] ACB_EXPORT auto
    GetFiles() const -> const std::map<std::uint32_t, AFS2_FILE_RECORD> &;

    [[nodiscard]] ACB_EXPORT auto GetStream() const -> IStream *;

    [[nodiscard]] ACB_EXPORT auto GetByteAlignment() const -> std::uint32_t;

    [[nodiscard]] ACB_EXPORT auto GetVersion() const -> std::uint32_t;

    [[nodiscard]] ACB_EXPORT auto GetHcaKeyModifier() const -> std::uint16_t;

    [[nodiscard]] ACB_EXPORT auto GetFileName() const -> const std::string &;

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

ACB_NS_END

#endif // ACB_ICHINOSE_CAFS2ARCHIVE_H_
