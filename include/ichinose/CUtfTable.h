#ifndef CGSS_ICHINOSE_CUTF_TABLE_H_
#define CGSS_ICHINOSE_CUTF_TABLE_H_

#include <cstdint>
#include <vector>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "ichinose/CUtfReader.h"
#include "takamori/streams/CMemoryStream.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CUtfTable {

    __root_class(CUtfTable);

public:
    CUtfTable(IStream *stream, std::uint64_t streamOffset);

    virtual ~CUtfTable();

    void GetHeader(UTF_HEADER &header) const;

    auto GetHeader() const -> const UTF_HEADER;

    auto GetStream() const -> IStream *;

    auto IsEncrypted() const -> bool_t;

    struct UtfRow {
        std::uint32_t baseOffset;
        std::vector<UTF_FIELD *> fields;
    };

    const std::vector<CUtfTable::UtfRow> &GetRows() const;

    auto GetName() const -> const char *;

    auto GetFieldOffset(std::uint32_t rowIndex, const char *fieldName, std::uint64_t *offset) const
        -> bool_t;

    auto GetFieldSize(std::uint32_t rowIndex, const char *fieldName, std::uint32_t *size) const
        -> bool_t;

protected:
    auto GetReader() const -> CUtfReader *;

    std::vector<CUtfTable::UtfRow> _rows;

    virtual void Initialize();

private:
    auto CheckEncryption(const std::uint8_t *magic) -> bool_t;

    static auto GetKeysForEncryptedUtfTable(
        const std::uint8_t *magic, _OUT_ std::uint8_t *seed, _OUT_ std::uint8_t *incr
    ) -> bool_t;

    static void ReadUtfHeader(IStream *stream, UTF_HEADER &header, char *tableNameBuffer);

    static void ReadUtfHeader(
        IStream *stream, std::uint64_t streamOffset, UTF_HEADER &header, char *tableNameBuffer
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
    char _tableName[UTF_TABLE_MAX_NAME_LEN];
};

CGSS_NS_END

#endif
