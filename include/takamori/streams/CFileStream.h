#ifndef CGSS_TAKAMORI_STREAMS_CFILESTREAM_H_
#define CGSS_TAKAMORI_STREAMS_CFILESTREAM_H_

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CFileStream final: public CStream {

    __extends(CStream, CFileStream);

public:
    explicit CFileStream(const char *fileName);

    CFileStream(const char *fileName, FileMode mode);

    CFileStream(const char *fileName, FileMode mode, FileAccess access);

    explicit CFileStream(const std::string &fileName);

    CFileStream(const std::string &fileName, FileMode mode);

    CFileStream(const std::string &fileName, FileMode mode, FileAccess access);

    CFileStream(const CFileStream &) = delete;

    CFileStream(CFileStream &&) = delete;

    auto operator=(const CFileStream &) -> CFileStream & = delete;

    auto operator=(CFileStream &&) -> CFileStream & = delete;

    ~CFileStream() override;

    auto Read(void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count)
        -> std::uint32_t override;

    auto Write(
        const void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count
    ) -> std::uint32_t override;

    auto IsWritable() const -> bool_t override;

    auto IsReadable() const -> bool_t override;

    auto IsSeekable() const -> bool_t override;

    auto GetPosition() -> std::uint64_t override;

    void SetPosition(std::uint64_t value) override;

    auto GetLength() -> std::uint64_t override;

    void SetLength(std::uint64_t value) override;

    void Flush() override;

private:
    auto OpenFile(const char *fileName) -> std::FILE *;

    static void CreateFileInternal(const char *fileName);

private:
    std::FILE *_fp;
    FileMode _mode;
    FileAccess _access;
    bool_t _isReadable, _isWritable, _isSeekable;
};

CGSS_NS_END

#endif // CGSS_TAKAMORI_STREAMS_CFILESTREAM_H_
