#ifndef CGSS_ICHINOSE_CUTF_TABLE_H_
#define CGSS_ICHINOSE_CUTF_TABLE_H_

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "ichinose/CUtfReader.h"
#include "takamori/streams/CMemoryStream.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

class CUtfTable {

    __root_class(CUtfTable);

public:
    CGSS_EXPORT CUtfTable(IStream *stream, std::uint64_t streamOffset);

    CGSS_EXPORT virtual ~CUtfTable();

    CGSS_EXPORT void GetHeader(UTF_HEADER &header) const;

    [[nodiscard]] CGSS_EXPORT auto GetHeader() const -> const UTF_HEADER;

    [[nodiscard]] CGSS_EXPORT auto GetStream() const -> IStream *;

    [[nodiscard]] CGSS_EXPORT auto IsEncrypted() const -> bool_t;

    struct UtfRow {
        std::uint32_t baseOffset;
        std::vector<UTF_FIELD *> fields;
    };

    [[nodiscard]] CGSS_EXPORT auto GetRows() const -> const std::vector<CUtfTable::UtfRow> &;

    [[nodiscard]] CGSS_EXPORT auto GetName() const -> std::string;

    CGSS_EXPORT auto GetFieldOffset(
        std::uint32_t rowIndex, const char *fieldName, std::uint64_t *offset
    ) const -> bool_t;

    CGSS_EXPORT auto GetFieldSize(
        std::uint32_t rowIndex, const char *fieldName, std::uint32_t *size
    ) const -> bool_t;

protected:
    [[nodiscard]] auto GetReader() const -> CUtfReader *;

    std::vector<CUtfTable::UtfRow> _rows;

    void Initialize();

private:
    auto CheckEncryption(const std::array<std::uint8_t, 4> &magic) -> bool_t;

    static auto GetKeysForEncryptedUtfTable(
        const std::array<std::uint8_t, 4> &magic, _OUT_ std::uint8_t *seed, _OUT_ std::uint8_t *incr
    ) -> bool_t;

    static void ReadUtfHeader(IStream *stream, UTF_HEADER &header, std::string &tableNameBuffer);

    static void ReadUtfHeader(
        IStream *stream,
        std::uint64_t streamOffset,
        UTF_HEADER &header,
        std::string &tableNameBuffer
    );

    void InitializeUtfSchema(
        IStream *sourceStream, CMemoryStream *tableDataStream, std::uint64_t schemaOffset
    );

    auto GetTableDataStream() -> CMemoryStream *;

    UTF_HEADER _utfHeader;
    IStream *_stream;
    bool_t _isEncrypted;
    std::uint64_t _streamOffset;
    CUtfReader *_utfReader;
    std::string _tableName;
};

CGSS_NS_END

#endif
