#ifndef ACB_ICHINOSE_CUTF_TABLE_H_
#define ACB_ICHINOSE_CUTF_TABLE_H_

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "acb_cdata.h"
#include "acb_env.h"
#include "acb_env_ns.h"
#include "ichinose/CUtfField.h"
#include "ichinose/CUtfReader.h"
#include "takamori/streams/CMemoryStream.h"
#include "takamori/streams/IStream.h"

ACB_NS_BEGIN

class CUtfTable {

    _root_class(CUtfTable);

public:
    ACB_EXPORT CUtfTable(IStream *stream, std::uint64_t streamOffset);

    ACB_EXPORT virtual ~CUtfTable();

    ACB_EXPORT void GetHeader(UTF_HEADER &header) const;

    [[nodiscard]] ACB_EXPORT auto GetHeader() const -> const UTF_HEADER;

    [[nodiscard]] ACB_EXPORT auto GetStream() const -> IStream *;

    [[nodiscard]] ACB_EXPORT auto IsEncrypted() const -> bool_t;

    struct UtfRow {
        std::uint32_t baseOffset;
        std::vector<CUtfField> fields;
    };

    [[nodiscard]] ACB_EXPORT auto GetRows() const -> const std::vector<CUtfTable::UtfRow> &;

    [[nodiscard]] ACB_EXPORT auto GetName() const -> std::string;

    ACB_EXPORT auto GetFieldOffset(
        std::uint32_t rowIndex, const char *fieldName, std::uint64_t *offset
    ) const -> bool_t;

    ACB_EXPORT auto GetFieldSize(std::uint32_t rowIndex, const char *fieldName, std::uint32_t *size)
        const -> bool_t;

protected:
    [[nodiscard]] auto GetReader() const -> CUtfReader *;

    std::vector<CUtfTable::UtfRow> _rows;

    void Initialize();

private:
    auto CheckEncryption(const std::array<std::uint8_t, 4> &magic) -> bool_t;

    static auto GetKeysForEncryptedUtfTable(
        const std::array<std::uint8_t, 4> &magic, std::uint8_t *seed, std::uint8_t *incr
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

ACB_NS_END

#endif
