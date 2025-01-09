#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "acb_cdata.h"
#include "acb_env_ns.h"
#include "ichinose/CAcbHelper.h"
#include "ichinose/CUtfField.h"
#include "ichinose/CUtfReader.h"
#include "ichinose/CUtfTable.h"
#include "takamori/exceptions/CFormatException.h"
#include "takamori/streams/CBinaryReader.h"
#include "takamori/streams/CMemoryStream.h"
#include "takamori/streams/CStreamExtensions.h"
#include "takamori/streams/IStream.h"

ACB_NS_BEGIN

constexpr std::array<std::uint8_t, 4> UTF_SIGNATURE = {'@', 'U', 'T', 'F'};

CUtfTable::CUtfTable(IStream *stream, std::uint64_t streamOffset)
    : _stream(stream), _streamOffset(streamOffset) {
    Initialize();
}

CUtfTable::~CUtfTable() {
    for (auto &row : _rows) {
        for (auto pField : row.fields) {
            delete pField;
        }
    }

    if (_utfReader) {
        delete _utfReader;
        _utfReader = nullptr;
    }
}

auto CUtfTable::GetStream() const -> IStream * {
    return _stream;
}

void CUtfTable::GetHeader(UTF_HEADER &header) const {
    std::memcpy(&header, &_utfHeader, sizeof(UTF_HEADER));
}

auto CUtfTable::GetHeader() const -> const UTF_HEADER {
    return _utfHeader;
}

auto CUtfTable::IsEncrypted() const -> bool_t {
    return _isEncrypted;
}

auto CUtfTable::GetRows() const -> const std::vector<CUtfTable::UtfRow> & {
    return _rows;
}

auto CUtfTable::GetName() const -> std::string {
    return _tableName;
}

auto CUtfTable::GetReader() const -> CUtfReader * {
    return _utfReader;
}

void CUtfTable::Initialize() {
    auto *stream      = _stream;
    const auto offset = _streamOffset;

    if (offset != stream->GetPosition()) {
        stream->Seek(offset, StreamSeekOrigin::Begin);
    }

    std::array<std::uint8_t, 4> magic;
    CBinaryReader::PeekBytes(stream, magic.data(), magic.size(), 0, 4);

    const auto magicFound = CheckEncryption(magic);

    if (!magicFound) {
        throw CFormatException("\"@UTF\" is not found.");
    }

    auto *tableDataStream = GetTableDataStream();
    auto &header          = _utfHeader;

    ReadUtfHeader(tableDataStream, header, _tableName);

    _rows.reserve(header.rowCount);

    if (header.tableSize > 0) {
        InitializeUtfSchema(stream, tableDataStream, 0x20);
    }

    delete tableDataStream;
}

auto CUtfTable::CheckEncryption(const std::array<std::uint8_t, 4> &magic) -> bool_t {
    if (magic == UTF_SIGNATURE) {
        _utfReader   = new CUtfReader();
        _isEncrypted = FALSE;
    } else {
        _isEncrypted = TRUE;
        std::uint8_t seed, increment;
        const auto keysFound = GetKeysForEncryptedUtfTable(magic, &seed, &increment);
        if (!keysFound) {
            return FALSE;
        } else {
            _utfReader = new CUtfReader(seed, increment);
        }
    }
    return TRUE;
}

auto CUtfTable::GetKeysForEncryptedUtfTable(
    const std::array<std::uint8_t, 4> &magic, std::uint8_t *seed, std::uint8_t *incr
) -> bool_t {
    for (auto s = 0; s <= 0xff; ++s) {
        if ((magic[0] ^ s) != UTF_SIGNATURE[0]) {
            continue;
        }
        for (auto i = 0; i <= 0xff; ++i) {
            auto m = s * i;
            if ((magic[1] ^ m) != UTF_SIGNATURE[1]) {
                continue;
            }
            auto t = i;
            for (auto j = 2; j < 4; ++j) {
                m *= t;
                if ((magic[j] ^ m) != UTF_SIGNATURE[j]) {
                    break;
                }
                if (j < 3) {
                    continue;
                }
                if (seed) {
                    *seed = static_cast<std::uint8_t>(s);
                }
                if (incr) {
                    *incr = static_cast<std::uint8_t>(i);
                }
                return TRUE;
            }
        }
    }
    return FALSE;
}

auto CUtfTable::GetTableDataStream() -> CMemoryStream * {
    auto *stream            = _stream;
    const auto streamOffset = _streamOffset;
    auto *reader            = _utfReader;
    const auto tableSize    = reader->PeekUInt32(stream, streamOffset, 4) + 8;

    if (!IsEncrypted()) {
        return CAcbHelper::ExtractToNewStream(stream, streamOffset, tableSize);
    }

    auto originalPosition        = stream->GetPosition();
    std::uint32_t totalBytesRead = 0;

    auto *memoryStream = new CMemoryStream(tableSize, FALSE);
    auto currentOffset = streamOffset;
    std::uint8_t *buffer;

    do {
        auto shouldRead = tableSize - totalBytesRead;
        buffer          = new std::uint8_t[shouldRead];

        _utfReader->PeekBytes(stream, buffer, 0, currentOffset, shouldRead, totalBytesRead);
        memoryStream->Write(buffer, shouldRead, 0, shouldRead);

        // TODO: Actually this should be "+= buffer.length", but now CUtfReader::PeekBytes() does
        // not return number of bytes read.
        currentOffset += shouldRead;
        totalBytesRead += shouldRead;

        delete[] buffer;
    } while (totalBytesRead < tableSize);

    stream->SetPosition(originalPosition);

    memoryStream->Seek(0, StreamSeekOrigin::Begin);

    return memoryStream;
}

void CUtfTable::ReadUtfHeader(IStream *stream, UTF_HEADER &header, std::string &tableNameBuffer) {
    ReadUtfHeader(stream, stream->GetPosition(), header, tableNameBuffer);
}

void CUtfTable::ReadUtfHeader(
    IStream *stream, std::uint64_t streamOffset, UTF_HEADER &header, std::string &tableNameBuffer
) {
    CBinaryReader reader(stream);
    auto pos = stream->GetPosition();

    reader.Seek(streamOffset + 4, StreamSeekOrigin::Begin);

    header.tableSize         = reader.ReadUInt32BE();
    header.unk1              = reader.ReadUInt16BE();
    header.perRowDataOffset  = (std::uint32_t)reader.ReadUInt16BE() + 8;
    header.stringTableOffset = reader.ReadUInt32BE() + 8;
    header.extraDataOffset   = reader.ReadUInt32BE() + 8;
    header.tableNameOffset   = reader.ReadUInt32BE();
    header.fieldCount        = reader.ReadUInt16BE();
    header.rowSize           = reader.ReadUInt16BE();
    header.rowCount          = reader.ReadUInt32BE();

    stream->Seek(header.stringTableOffset + header.tableNameOffset, StreamSeekOrigin::Begin);
    CStreamExtensions::ReadNullEndedString(stream, tableNameBuffer, UTF_TABLE_MAX_NAME_LEN);

    stream->Seek(pos, StreamSeekOrigin::Begin);
}

void CUtfTable::InitializeUtfSchema(
    IStream *sourceStream, CMemoryStream *tableDataStream, std::uint64_t schemaOffset
) {
    const auto &header    = _utfHeader;
    const auto baseOffset = _streamOffset;
    auto &rows            = _rows;
    std::string fieldNameBuffer;

    for (std::uint32_t i = 0; i < header.rowCount; ++i) {
        auto currentStreamOffset = schemaOffset;
        UtfRow row;
        std::uint32_t currentRowOffset = 0;
        row.baseOffset                 = header.perRowDataOffset + header.rowSize * i;

        for (auto j = 0; j < header.fieldCount; ++j) {
            auto field            = new CUtfField();
            const auto columnType = CBinaryReader::PeekUInt8(tableDataStream, currentStreamOffset);
            const auto nameOffset =
                CBinaryReader::PeekInt32BE(tableDataStream, currentStreamOffset + 1);
            const auto pos = tableDataStream->GetPosition();
            tableDataStream->Seek(header.stringTableOffset + nameOffset, StreamSeekOrigin::Begin);
            CStreamExtensions::ReadNullEndedString(
                tableDataStream, fieldNameBuffer, UTF_FIELD_MAX_NAME_LEN
            );
            field->SetName(fieldNameBuffer);
            tableDataStream->Seek(pos, StreamSeekOrigin::Begin);

            const auto storage =
                static_cast<UtfColumnStorage>(columnType & ACB_UTF_COLUMN_STORAGE_MASK);
            const auto type = static_cast<UtfColumnType>(columnType & ACB_UTF_COLUMN_TYPE_MASK);

            field->type    = static_cast<ACB_UTF_COLUMN_TYPE>(type);
            field->storage = static_cast<ACB_UTF_COLUMN_STORAGE>(storage);

            switch (storage) {
            case UtfColumnStorage::Const:
            case UtfColumnStorage::Const2: {
                const auto constantOffset = static_cast<std::uint32_t>(currentStreamOffset) + 5;
                field->offsetInRow        = 0; // constant

                switch (type) {
                case UtfColumnType::U8:
                    field->SetValue(
                        CBinaryReader::PeekUInt8(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 1;
                    break;
                case UtfColumnType::S8:
                    field->SetValue(
                        CBinaryReader::PeekInt8(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 1;
                    break;
                case UtfColumnType::U16:
                    field->SetValue(
                        CBinaryReader::PeekUInt16BE(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 2;
                    break;
                case UtfColumnType::S16:
                    field->SetValue(
                        CBinaryReader::PeekInt16BE(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 2;
                    break;
                case UtfColumnType::U32:
                    field->SetValue(
                        CBinaryReader::PeekUInt32BE(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 4;
                    break;
                case UtfColumnType::S32:
                    field->SetValue(
                        CBinaryReader::PeekInt32BE(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 4;
                    break;
                case UtfColumnType::U64:
                    field->SetValue(
                        CBinaryReader::PeekUInt64BE(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 8;
                    break;
                case UtfColumnType::S64:
                    field->SetValue(
                        CBinaryReader::PeekInt64BE(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 8;
                    break;
                case UtfColumnType::R32:
                    field->SetValue(
                        CBinaryReader::PeekSingleBE(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 4;
                    break;
                case UtfColumnType::R64:
                    field->SetValue(
                        CBinaryReader::PeekDoubleBE(tableDataStream, constantOffset), constantOffset
                    );
                    currentStreamOffset += 8;
                    break;
                case UtfColumnType::String: {
                    const auto dataOffset =
                        CBinaryReader::PeekUInt32BE(tableDataStream, constantOffset);
                    const auto fieldDataOffset = header.stringTableOffset + dataOffset;
                    const auto *str = reinterpret_cast<const char *>(tableDataStream->GetBuffer()) +
                                      fieldDataOffset;
                    field->SetValue(str, dataOffset);
                    currentStreamOffset += 4;
                    break;
                }
                case UtfColumnType::Data: {
                    const auto dataOffset =
                        CBinaryReader::PeekUInt32BE(tableDataStream, constantOffset);
                    const auto dataSize =
                        CBinaryReader::PeekUInt32BE(tableDataStream, constantOffset + 4);
                    const auto fieldDataOffset = static_cast<std::uint32_t>(baseOffset) +
                                                 header.extraDataOffset + dataOffset;
                    if (dataSize > 0) {
                        auto dataBuffer = new std::uint8_t[dataSize];
                        std::memset(dataBuffer, 0, dataSize);
                        sourceStream->Seek(fieldDataOffset, StreamSeekOrigin::Begin);
                        sourceStream->Read(dataBuffer, dataSize, 0, dataSize);
                        field->SetValue(dataBuffer, dataSize, fieldDataOffset);
                        delete[] dataBuffer;
                    } else {
                        field->SetValue(nullptr, dataSize, fieldDataOffset);
                    }
                    currentStreamOffset += 8;
                    break;
                }
                default:
                    throw CFormatException("Unknown UTF table field type.");
                }
                break;
            }
            case UtfColumnStorage::PerRow: {
                auto fieldDataOffset = row.baseOffset + currentRowOffset;
                field->offsetInRow   = currentRowOffset;

                switch (type) {
                case UtfColumnType::U8:
                    field->SetValue(
                        CBinaryReader::PeekUInt8(tableDataStream, fieldDataOffset), fieldDataOffset
                    );
                    currentRowOffset += 1;
                    break;
                case UtfColumnType::S8:
                    field->SetValue(
                        CBinaryReader::PeekInt8(tableDataStream, fieldDataOffset), fieldDataOffset
                    );
                    currentRowOffset += 1;
                    break;
                case UtfColumnType::U16:
                    field->SetValue(
                        CBinaryReader::PeekUInt16BE(tableDataStream, fieldDataOffset),
                        fieldDataOffset
                    );
                    currentRowOffset += 2;
                    break;
                case UtfColumnType::S16:
                    field->SetValue(
                        CBinaryReader::PeekInt16BE(tableDataStream, fieldDataOffset),
                        fieldDataOffset
                    );
                    currentRowOffset += 2;
                    break;
                case UtfColumnType::U32:
                    field->SetValue(
                        CBinaryReader::PeekUInt32BE(tableDataStream, fieldDataOffset),
                        fieldDataOffset
                    );
                    currentRowOffset += 4;
                    break;
                case UtfColumnType::S32:
                    field->SetValue(
                        CBinaryReader::PeekInt32BE(tableDataStream, fieldDataOffset),
                        fieldDataOffset
                    );
                    currentRowOffset += 4;
                    break;
                case UtfColumnType::U64:
                    field->SetValue(
                        CBinaryReader::PeekInt64BE(tableDataStream, fieldDataOffset),
                        fieldDataOffset
                    );
                    currentRowOffset += 8;
                    break;
                case UtfColumnType::S64:
                    field->SetValue(
                        CBinaryReader::PeekInt64BE(tableDataStream, fieldDataOffset),
                        fieldDataOffset
                    );
                    currentRowOffset += 8;
                    break;
                case UtfColumnType::R32:
                    field->SetValue(
                        CBinaryReader::PeekSingleBE(tableDataStream, fieldDataOffset),
                        fieldDataOffset
                    );
                    currentRowOffset += 4;
                    break;
                case UtfColumnType::R64:
                    field->SetValue(
                        CBinaryReader::PeekDoubleBE(tableDataStream, fieldDataOffset),
                        fieldDataOffset
                    );
                    currentRowOffset += 8;
                    break;
                case UtfColumnType::String: {
                    const auto rowDataOffset = CBinaryReader::PeekUInt32BE(
                        tableDataStream, row.baseOffset + currentRowOffset
                    );
                    fieldDataOffset = header.stringTableOffset + rowDataOffset;
                    const auto *str = reinterpret_cast<const char *>(tableDataStream->GetBuffer()) +
                                      fieldDataOffset;
                    field->SetValue(str, fieldDataOffset);
                    currentRowOffset += 4;
                    break;
                }
                case UtfColumnType::Data: {
                    const auto rowDataOffset = CBinaryReader::PeekUInt32BE(
                        tableDataStream, row.baseOffset + currentRowOffset
                    );
                    const auto dataSize = CBinaryReader::PeekUInt32BE(
                        tableDataStream, row.baseOffset + currentRowOffset + 4
                    );
                    fieldDataOffset = static_cast<std::uint32_t>(baseOffset) +
                                      header.extraDataOffset + rowDataOffset;
                    if (dataSize > 0) {
                        auto dataBuffer = new std::uint8_t[dataSize];
                        std::memset(dataBuffer, 0, dataSize);
                        sourceStream->Seek(fieldDataOffset, StreamSeekOrigin::Begin);
                        sourceStream->Read(dataBuffer, dataSize, 0, dataSize);
                        field->SetValue(dataBuffer, dataSize, fieldDataOffset);
                        delete[] dataBuffer;
                    } else {
                        field->SetValue(nullptr, dataSize, fieldDataOffset);
                    }
                    currentRowOffset += 8;
                    break;
                }
                default:
                    throw CFormatException("Unknown UTF table field type.");
                }
                break;
            }
            default:
                throw CFormatException("Unknown UTF table field storage format.");
            }

            row.fields.push_back(field);

            currentStreamOffset += 5;
        }

        rows.push_back(row);
    }
}

auto CUtfTable::GetFieldOffset(std::uint32_t rowIndex, const char *fieldName, std::uint64_t *offset)
    const -> bool_t {
    if (rowIndex >= _rows.size()) {
        if (offset) {
            *offset = 0;
        }

        return FALSE;
    }

    const auto &row = _rows[rowIndex];

    for (auto &field : row.fields) {
        if (std::strcmp(fieldName, field->name) == 0) {
            if (offset) {
                *offset = field->offset;
            }

            return TRUE;
        }
    }

    return FALSE;
}

auto CUtfTable::GetFieldSize(std::uint32_t rowIndex, const char *fieldName, std::uint32_t *size)
    const -> bool_t {
    if (rowIndex >= _rows.size()) {
        if (size) {
            *size = 0;
        }

        return FALSE;
    }

    const auto &row = _rows[rowIndex];

    for (auto &field : row.fields) {
        if (std::strcmp(fieldName, field->name) == 0) {
            if (size) {
                *size = field->value.data.size;
            }

            return TRUE;
        }
    }

    return FALSE;
}

ACB_NS_END
