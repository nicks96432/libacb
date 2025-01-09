#include <cstdint>
#include <cstdio>
#include <cstring>
#include <format>
#include <string>
#include <type_traits>
#include <vector>

#include "cgss_cdata.h"
#include "cgss_cenum.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "cgss_env_platform.h"
#include "ichinose/CAcbFile.h"
#include "ichinose/CAcbHelper.h"
#include "ichinose/CAfs2Archive.h"
#include "takamori/CFileSystem.h"
#include "takamori/CPath.h"
#include "takamori/exceptions/CArgumentException.h"
#include "takamori/exceptions/CFormatException.h"
#include "takamori/exceptions/CInvalidOperationException.h"
#include "takamori/streams/CBinaryReader.h"
#include "takamori/streams/CFileStream.h"
#include "takamori/streams/IStream.h"

CGSS_NS_BEGIN

#define DEFAULT_BINARY_FILE_EXTENSION ".bin"
static const std::string &DefaultBinaryFileExtension = DEFAULT_BINARY_FILE_EXTENSION;

static auto GetExtensionForEncodeType(std::uint8_t encodeType) -> std::string;

template<typename T>
auto GetFieldValueAsNumber(
    CUtfTable *table, std::uint32_t rowIndex, const char *fieldName, T *result
) -> bool_t requires(std::is_arithmetic_v<T>);

auto GetFieldValueAsString(
    CUtfTable *table, std::uint32_t rowIndex, const char *fieldName, std::string &s
) -> bool_t;

CAcbFile::CAcbFile(cgss::IStream *stream, const char *fileName): MyClass(stream, 0, fileName) {}

CAcbFile::CAcbFile(IStream *stream, std::uint64_t streamOffset, const char *fileName)
    : MyBase(stream, streamOffset) {
    _internalAwb   = nullptr;
    _externalAwb   = nullptr;
    _fileName      = fileName;
    _formatVersion = 0;
}

CAcbFile::~CAcbFile() {
    for (const auto &pair : _tables) {
        delete pair.second;
    }

    delete _internalAwb;
    _internalAwb = nullptr;
    delete _externalAwb;
    _externalAwb = nullptr;
}

void CAcbFile::Initialize() {
    MyBase::Initialize();

    GetFieldValueAsNumber(this, 0, "Version", &_formatVersion);

    InitializeCueList();
    InitializeCueNameToWaveformMap();
    InitializeTrackList();
    InitializeAwbArchives();
}

void CAcbFile::InitializeCueList() {
    auto cueTable = GetTable("CueTable");

    if (cueTable == nullptr) {
        throw CFormatException("Missing 'Cue' table.");
    }

    auto waveformTable = GetTable("WaveformTable");

    if (waveformTable == nullptr) {
        throw CFormatException("Missing 'Waveform' table.");
    }

    auto synthTable = GetTable("SynthTable");

    if (synthTable == nullptr) {
        throw CFormatException("Missing 'Synth' table.");
    }

    const auto cueCount = cueTable->GetRows().size();

    std::uint64_t refItemOffset     = 0;
    std::uint32_t refItemSize       = 0;
    std::uint32_t refSizeCorrection = 0;

    auto &cues = this->_cues;

    cues.reserve(cueCount);

    for (std::uint32_t i = 0; i < cueCount; ++i) {
        ACB_CUE_RECORD cue = {};

        cue.isWaveformIdentified = FALSE;
        GetFieldValueAsNumber(cueTable, i, "CueId", &cue.cueId);
        GetFieldValueAsNumber(cueTable, i, "ReferenceType", &cue.referenceType);

#ifdef __CGSS_OS_WINDOWS__
#pragma warning(push)
#pragma warning(disable: 4366)
#endif
        GetFieldValueAsNumber(cueTable, i, "ReferenceIndex", &cue.referenceIndex);
#ifdef __CGSS_OS_WINDOWS__
#pragma warning(pop)
#endif

        switch (cue.referenceType) {
        case 2:
            synthTable->GetFieldOffset(cue.referenceIndex, "ReferenceItems", &refItemOffset);
            synthTable->GetFieldSize(cue.referenceIndex, "ReferenceItems", &refItemSize);
            refSizeCorrection = refItemSize + 2;
            break;
        case 3:
        case 8:
            if (i == 0) {
                synthTable->GetFieldOffset(0, "ReferenceItems", &refItemOffset);
                synthTable->GetFieldSize(0, "ReferenceItems", &refItemSize);
                refSizeCorrection = refItemSize - 2;
            } else {
                refSizeCorrection += 4;
            }
            break;
        default:
            throw CFormatException("Unknown reference type.");
        }

        if (refItemSize != 0) {
            CBinaryReader reader(GetStream());

            cue.waveformIndex = reader.PeekUInt16BE(refItemOffset + refSizeCorrection);

            std::uint8_t isStreaming;
            auto hasIsStreaming =
                GetFieldValueAsNumber(waveformTable, cue.waveformIndex, "Streaming", &isStreaming);

            if (hasIsStreaming) {
                cue.isStreaming = isStreaming;

                std::uint16_t waveformId;

                if (GetFieldValueAsNumber(waveformTable, cue.waveformIndex, "Id", &waveformId)) {
                    cue.waveformId = waveformId;
                } else {
                    if (cue.isStreaming) {
                        if (GetFieldValueAsNumber(
                                waveformTable, cue.waveformIndex, "StreamAwbId", &waveformId
                            )) {
                            cue.waveformId = waveformId;
                        }
                    } else {
                        if (GetFieldValueAsNumber(
                                waveformTable, cue.waveformIndex, "MemoryAwbId", &waveformId
                            )) {
                            cue.waveformId = waveformId;
                        }
                    }
                }

                std::uint8_t encodeType;
                if (GetFieldValueAsNumber(
                        waveformTable, cue.waveformIndex, "EncodeType", &encodeType
                    )) {
                    cue.encodeType = encodeType;
                }

                cue.isWaveformIdentified = TRUE;
            }
        }

        cues.push_back(cue);
    }
}

void CAcbFile::InitializeCueNameToWaveformMap() {
    auto cueNameTable = GetTable("CueNameTable");

    if (cueNameTable == nullptr) {
        throw CFormatException("Missing 'CueName' table.");
    }

    auto cueNameCount = cueNameTable->GetRows().size();
    for (std::uint32_t i = 0; i < cueNameCount; ++i) {
        std::uint16_t cueIndex;

        if (!GetFieldValueAsNumber(cueNameTable, i, "CueIndex", &cueIndex)) {
            continue;
        }

        auto &cue = this->_cues[cueIndex];

        if (cue.isWaveformIdentified) {
            std::string cueName;

            if (!GetFieldValueAsString(cueNameTable, i, "CueName", cueName)) {
                continue;
            }

            cueName += GetExtensionForEncodeType(cue.encodeType);

            std::snprintf(cue.cueName, ACB_CUE_RECORD_NAME_MAX_LEN, "%s", cueName.c_str());

            _cueNameToWaveform[cueName] = cue.waveformId;

            _fileNames.emplace_back(cue.cueName);
        }
    }
}

// TODO: remove duplicate code (see InitializeCueList())
void CAcbFile::InitializeTrackList() {
    auto trackTable = GetTable("TrackTable");

    if (trackTable == nullptr) {
        throw CFormatException("Missing 'Track' table.");
    }

    auto waveformTable = GetTable("WaveformTable");

    if (waveformTable == nullptr) {
        throw CFormatException("Missing 'Waveform' table.");
    }

    auto synthTable = GetTable("SynthTable");

    if (synthTable == nullptr) {
        throw CFormatException("Missing 'Synth' table.");
    }

    const auto trackCount = trackTable->GetRows().size();
    const auto &synthRows = synthTable->GetRows();
    const auto synthCount = synthRows.size();

    if (trackCount != synthCount) {
        throw CFormatException("Number of tracks and number of synthesis records do not match.");
    }

    std::uint64_t refItemOffset     = 0;
    std::uint32_t refItemSize       = 0;
    std::uint32_t refSizeCorrection = 0;

    CBinaryReader reader(GetStream());
    auto &tracks = _tracks;

    tracks.reserve(trackCount);

    for (std::uint32_t i = 0; i < trackCount; ++i) {
        ACB_TRACK_RECORD track = {};

        track.isWaveformIdentified = FALSE;
        track.trackIndex           = i;
        track.synthIndex           = static_cast<std::uint16_t>(track.trackIndex);

        const auto &synthRow    = synthRows[i];
        UTF_FIELD *refItemField = nullptr;

        for (const auto &field : synthRow.fields) {
            if (std::strcmp(field->name, "ReferenceItems") == 0) {
                refItemField = field;
                break;
            }
        }

        if (refItemField == nullptr) {
            throw CFormatException("Missing 'ReferenceItems' field in row.");
        }

        bool isStoredPerRow;

        switch (refItemField->storage) {
        case CGSS_UTF_COLUMN_STORAGE_PER_ROW:
            isStoredPerRow = true;
            break;
        case CGSS_UTF_COLUMN_STORAGE_CONST:
        case CGSS_UTF_COLUMN_STORAGE_CONST2:
            isStoredPerRow = false;
            break;
        default:
            throw CFormatException("Unsupported field storage type for 'ReferenceItems' field.");
        }

        if (isStoredPerRow) {
            synthTable->GetFieldOffset(track.synthIndex, "ReferenceItems", &refItemOffset);
            synthTable->GetFieldSize(track.synthIndex, "ReferenceItems", &refItemSize);
            // TODO: I think this should be -2 instead of +2 when looping through synth records.
            refSizeCorrection = refItemSize - 2;
        } else {
            if (i == 0) {
                synthTable->GetFieldOffset(0, "ReferenceItems", &refItemOffset);
                synthTable->GetFieldSize(0, "ReferenceItems", &refItemSize);
                refSizeCorrection = refItemSize - 2;
            } else {
                refSizeCorrection += 4;
            }
        }

        if (refItemSize != 0) {
            track.waveformIndex = reader.PeekUInt16BE(refItemOffset + refSizeCorrection);

            std::uint8_t isStreaming;
            auto hasIsStreaming = GetFieldValueAsNumber(
                waveformTable, track.waveformIndex, "Streaming", &isStreaming
            );

            if (hasIsStreaming) {
                track.isStreaming = isStreaming;

                std::uint16_t waveformId;

                if (GetFieldValueAsNumber(waveformTable, track.waveformIndex, "Id", &waveformId)) {
                    track.waveformId = waveformId;
                } else {
                    if (track.isStreaming) {
                        if (GetFieldValueAsNumber(
                                waveformTable, track.waveformIndex, "StreamAwbId", &waveformId
                            )) {
                            track.waveformId = waveformId;
                        }
                    } else {
                        if (GetFieldValueAsNumber(
                                waveformTable, track.waveformIndex, "MemoryAwbId", &waveformId
                            )) {
                            track.waveformId = waveformId;
                        }
                    }
                }

                std::uint8_t encodeType;
                if (GetFieldValueAsNumber(
                        waveformTable, track.waveformIndex, "EncodeType", &encodeType
                    )) {
                    track.encodeType = encodeType;
                }

                track.isWaveformIdentified = TRUE;
            }
        }

        tracks.push_back(track);
    }
}

void CAcbFile::InitializeAwbArchives() {
    std::uint32_t internalAwbSize;
    if (GetFieldSize(0, "AwbFile", &internalAwbSize) && internalAwbSize > 0) {
        _internalAwb = GetInternalAwb();
    }

    std::uint32_t externalAwbSize;
    if (GetFieldSize(0, "StreamAwbAfs2Header", &externalAwbSize) && externalAwbSize > 0) {
        _externalAwb = GetExternalAwb();
    }
}

auto CAcbFile::GetTable(const char *tableName) const -> CUtfTable * {
    std::string s(tableName);

    CUtfTable *table;

    if (_tables.find(s) != _tables.end()) {
        table = _tables[s];
    } else {
        table = ResolveTable(tableName);

        if (table) {
            _tables[s] = table;
        }
    }

    return table;
}

auto CAcbFile::ResolveTable(const char *tableName) const -> CUtfTable * {
    std::uint64_t tableOffset;

    if (!GetFieldOffset(0, tableName, &tableOffset)) {
        return nullptr;
    }

    std::uint32_t tableSize;

    if (!GetFieldSize(0, tableName, &tableSize)) {
        return nullptr;
    }

    auto tbl = new CUtfTable(GetStream(), tableOffset);

    return tbl;
}

auto CAcbFile::GetInternalAwb() const -> const CAfs2Archive * {
    std::uint64_t internalAwbOffset;

    if (!GetFieldOffset(0, "AwbFile", &internalAwbOffset) || internalAwbOffset == 0) {
        return nullptr;
    }

    std::uint32_t internalAwbSize;

    if (!GetFieldSize(0, "AwbFile", &internalAwbSize) || internalAwbSize == 0) {
        return nullptr;
    }

    auto internalAwb = new CAfs2Archive(GetStream(), internalAwbOffset, GetFileName(), FALSE);

    return internalAwb;
}

auto CAcbFile::GetExternalAwb() const -> const CAfs2Archive * {
    const auto extAwbFileName = FindExternalAwbFileName();

    if (extAwbFileName.empty()) {
        return nullptr;
    }

    // Checksum checking is skipped.

    const auto fs =
        new CFileStream(extAwbFileName.c_str(), FileMode::OpenExisting, FileAccess::Read);
    const auto archive = new CAfs2Archive(fs, 0, extAwbFileName.c_str(), TRUE);

    return archive;
}

auto CAcbFile::GetFileNames() const -> const std::vector<std::string> & {
    return _fileNames;
}

auto CAcbFile::OpenDataStream(const AFS2_FILE_RECORD *fileRecord, bool_t isStreaming) const
    -> IStream * {
    IStream *stream;

    if (isStreaming) {
        if (_externalAwb) {
            stream = _externalAwb->GetStream();
        } else {
            stream = nullptr;
        }
    } else {
        stream = this->GetStream();
    }

    auto result = CAcbHelper::ExtractToNewStream(
        stream, fileRecord->fileOffsetAligned, static_cast<std::uint32_t>(fileRecord->fileSize)
    );

    return result;
}

auto CAcbFile::OpenDataStream(const char *fileName) const -> IStream * {
    IStream *result = nullptr;

    const auto cue = GetCueRecordByWaveformFileName(fileName);

    if (cue) {
        const auto file   = GetFileRecordByCueRecord(cue);
        const auto stream = ChooseSourceStream(cue);

        result = CAcbHelper::ExtractToNewStream(
            stream, file->fileOffsetAligned, static_cast<std::uint32_t>(file->fileSize)
        );
    }

    return result;
}

auto CAcbFile::OpenDataStream(std::uint32_t cueId) const -> IStream * {
    IStream *result = nullptr;

    const auto cue = GetCueRecordByCueId(cueId);

    if (cue) {
        const auto file   = GetFileRecordByCueRecord(cue);
        const auto stream = ChooseSourceStream(cue);

        result = CAcbHelper::ExtractToNewStream(
            stream, file->fileOffsetAligned, static_cast<std::uint32_t>(file->fileSize)
        );
    }

    return result;
}

auto CAcbFile::GetFileName() const -> const char * {
    return _fileName;
}

auto CAcbFile::GetSymbolicFileNameHintByCueId(std::uint32_t cueId) const -> std::string {
    auto baseName = GetSymbolicFileBaseNameByCueId(cueId);

    const auto extHint = GetFileExtensionHintByCueId(cueId);
    const auto &ext    = extHint.empty() ? DefaultBinaryFileExtension : extHint;

    return baseName + ext;
}

auto CAcbFile::GetSymbolicFileBaseNameByCueId(std::uint32_t cueId) -> std::string {
    return std::format("cue_{:06d}", cueId);
}

auto CAcbFile::GetSymbolicFileNameHintByTrackIndex(std::uint32_t trackIndex) const -> std::string {
    auto baseName = GetSymbolicFileBaseNameByTrackIndex(trackIndex);

    const auto extHint = GetFileExtensionHintByTrackIndex(trackIndex);
    const auto &ext    = extHint.empty() ? DefaultBinaryFileExtension : extHint;

    return baseName + ext;
}

auto CAcbFile::GetSymbolicFileBaseNameByTrackIndex(std::uint32_t trackIndex) -> std::string {
    return std::format("track_{:06d}", trackIndex);
}

auto CAcbFile::GetCueNameByCueId(std::uint32_t cueId) const -> std::string {
    for (const auto &cue : _cues) {
        if (cue.waveformId == cueId) {
            return {cue.cueName};
        }
    }

    return GetSymbolicFileNameHintByCueId(cueId);
}

auto CAcbFile::GetCueNameByTrackIndex(std::uint32_t trackIndex) const -> std::string {
    auto track = GetTrackRecordByTrackIndex(trackIndex);

    if (track != nullptr) {
        for (const auto &cue : _cues) {
            if (cue.waveformId == track->waveformId) {
                return {cue.cueName};
            }
        }
    }

    return GetSymbolicFileNameHintByTrackIndex(trackIndex);
}

auto CAcbFile::GetCueRecordByWaveformFileName(const char *waveformFileName
) const -> const ACB_CUE_RECORD * {
    for (const auto &cue : _cues) {
        if (std::strcmp(cue.cueName, waveformFileName) == 0) {
            return &cue;
        }
    }

    return nullptr;
}

auto CAcbFile::GetCueRecordByCueId(std::uint32_t cueId) const -> const ACB_CUE_RECORD * {
    for (const auto &cue : _cues) {
        if (cue.cueId == cueId) {
            return &cue;
        }
    }

    return nullptr;
}

auto CAcbFile::GetFileRecordByWaveformFileName(const char *waveformFileName
) const -> const AFS2_FILE_RECORD * {
    const auto cue = GetCueRecordByWaveformFileName(waveformFileName);

    if (cue == nullptr) {
        return nullptr;
    }

    return GetFileRecordByCueRecord(cue);
}

auto CAcbFile::GetFileRecordByCueId(std::uint32_t cueId) const -> const AFS2_FILE_RECORD * {
    const auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return nullptr;
    }

    return GetFileRecordByCueRecord(cue);
}

auto CAcbFile::GetFileRecordByTrackIndex(std::uint32_t trackIndex
) const -> const AFS2_FILE_RECORD * {
    const ACB_TRACK_RECORD *track = nullptr;

    for (const auto &trackRecord : _tracks) {
        if (trackRecord.trackIndex == trackIndex) {
            track = &trackRecord;
            break;
        }
    }

    if (track == nullptr) {
        return nullptr;
    }

    // TODO: remove duplicate code (see GetFileRecordByCueRecord())
    if (!track->isWaveformIdentified) {
        return nullptr;
    }

    if (track->isStreaming) {
        auto externalAwb = _externalAwb;

        if (externalAwb == nullptr) {
            return nullptr;
        }

        auto &files = externalAwb->GetFiles();
        if (files.find(track->waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(track->waveformId);

        return &file;
    } else {
        auto internalAwb = _internalAwb;

        if (internalAwb == nullptr) {
            return nullptr;
        }

        auto &files = internalAwb->GetFiles();

        if (files.find(track->waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(track->waveformId);

        return &file;
    }
}

auto CAcbFile::GetTrackCountOfCueByCueId(std::uint32_t cueId) const -> std::uint32_t {
    auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return 0;
    }

    auto sequenceTable = GetTable("SequenceTable");

    if (sequenceTable == nullptr) {
        throw CFormatException("Missing 'Sequence' table.");
    }

    std::uint16_t numTracks = 0;

    GetFieldValueAsNumber(sequenceTable, cue->referenceIndex, "NumTracks", &numTracks);

    return numTracks;
}

auto CAcbFile::GetTrackIndicesOfCueByCueId(
    std::uint32_t cueId, std::uint32_t *numberOfTracks, std::uint32_t *trackIndices
) const -> bool_t {
    if (trackIndices == nullptr) {
        if (numberOfTracks != nullptr) {
            *numberOfTracks = GetTrackCountOfCueByCueId(cueId);
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (numberOfTracks == nullptr) {
        throw CArgumentException("Pointer to number of tracks cannot be null.");
    }

    const auto givenNumberOfTracks = *numberOfTracks;
    *numberOfTracks                = 0;

    const auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return FALSE;
    }

    const auto sequenceTable = GetTable("SequenceTable");

    if (sequenceTable == nullptr) {
        throw CFormatException("Missing 'Sequence' table.");
    }

    const auto expectedNumberOfTracks = GetTrackCountOfCueByCueId(cueId);

    *numberOfTracks = expectedNumberOfTracks;

    if (givenNumberOfTracks < expectedNumberOfTracks) {
        return FALSE;
    }

    std::uint64_t trackIndexOffset = 0;
    std::uint32_t trackIndexSize   = 0;
    sequenceTable->GetFieldOffset(cue->referenceIndex, "TrackIndex", &trackIndexOffset);
    sequenceTable->GetFieldSize(cue->referenceIndex, "TrackIndex", &trackIndexSize);

    if (trackIndexSize != expectedNumberOfTracks * sizeof(std::uint16_t)) {
        throw CFormatException(
            "Track index data error: data size and declared count does not match."
        );
    }

    CBinaryReader reader(GetStream());

    for (std::uint32_t i = 0; i < expectedNumberOfTracks; i += 1) {
        auto trackIndex = reader.PeekUInt16BE(trackIndexOffset + sizeof(std::uint16_t) * i);
        trackIndices[i] = trackIndex;
    }

    return TRUE;
}

auto CAcbFile::GetTrackIndicesOfCueByCueId(
    std::uint32_t cueId, std::vector<std::uint32_t> &trackIndices
) const -> bool_t {
    const auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return FALSE;
    }

    const auto sequenceTable = GetTable("SequenceTable");

    if (sequenceTable == nullptr) {
        throw CFormatException("Missing 'Sequence' table.");
    }

    const auto expectedNumberOfTracks = GetTrackCountOfCueByCueId(cueId);

    std::uint64_t trackIndexOffset = 0;
    std::uint32_t trackIndexSize   = 0;
    sequenceTable->GetFieldOffset(cue->referenceIndex, "TrackIndex", &trackIndexOffset);
    sequenceTable->GetFieldSize(cue->referenceIndex, "TrackIndex", &trackIndexSize);

    if (trackIndexSize != expectedNumberOfTracks * sizeof(std::uint16_t)) {
        throw CFormatException(
            "Track index data error: data size and declared count does not match."
        );
    }

    trackIndices.resize(expectedNumberOfTracks);

    CBinaryReader reader(GetStream());

    for (std::uint32_t i = 0; i < expectedNumberOfTracks; i += 1) {
        auto trackIndex = reader.PeekUInt16BE(trackIndexOffset + sizeof(std::uint16_t) * i);
        trackIndices[i] = trackIndex;
    }

    return TRUE;
}

auto CAcbFile::GetTrackRecords() const -> const std::vector<ACB_TRACK_RECORD> & {
    return _tracks;
}

auto CAcbFile::IsCueIdentified(std::uint32_t cueId) const -> bool_t {
    for (auto &cue : _cues) {
        if (cue.waveformId == cueId) {
            return cue.isWaveformIdentified;
        }
    }

    return FALSE;
}

auto CAcbFile::GetFormatVersion() const -> std::uint32_t {
    return _formatVersion;
}

auto CAcbFile::GetFileExtensionHintByCueId(std::uint32_t cueId) const -> std::string {
    const auto cue = GetCueRecordByCueId(cueId);

    if (cue == nullptr) {
        return "";
    } else {
        return GetExtensionForEncodeType(cue->encodeType);
    }
}

auto CAcbFile::GetFileExtensionHintByWaveformFileName(const char *waveformFileName
) const -> std::string {
    const auto cue = GetCueRecordByWaveformFileName(waveformFileName);

    if (cue == nullptr) {
        return "";
    } else {
        return GetExtensionForEncodeType(cue->encodeType);
    }
}

auto CAcbFile::GetFileExtensionHintByTrackIndex(std::uint32_t trackIndex) const -> std::string {
    auto track = GetTrackRecordByTrackIndex(trackIndex);

    if (track == nullptr) {
        return DefaultBinaryFileExtension;
    } else {
        return GetExtensionForEncodeType(track->encodeType);
    }
}

auto CAcbFile::GetTrackRecordByTrackIndex(std::uint32_t trackIndex
) const -> const ACB_TRACK_RECORD * {
    for (const auto &track : _tracks) {
        if (track.trackIndex == trackIndex) {
            return &track;
        }
    }

    return nullptr;
}

auto CAcbFile::FindExternalAwbFileName() const -> std::string {
    const std::string acbFileName = _fileName;
    const auto awbDirPath         = CPath::GetDirectoryName(acbFileName);

    auto awbBaseFileName = CPath::GetFileBaseName(acbFileName);

    awbBaseFileName = CPath::Combine(awbDirPath, awbBaseFileName);

    auto test = awbBaseFileName + "_streamfiles.awb";

    if (CFileSystem::FileExists(test.c_str())) {
        return test;
    }

    test = awbBaseFileName + ".awb";

    if (CFileSystem::FileExists(test.c_str())) {
        return test;
    }

    test = awbBaseFileName + "_STR.awb";

    if (CFileSystem::FileExists(test.c_str())) {
        return test;
    }

    return "";
}

auto CAcbFile::GetFileRecordByCueRecord(const ACB_CUE_RECORD *cue
) const -> const AFS2_FILE_RECORD * {
    if (cue == nullptr) {
        return nullptr;
    }

    if (!cue->isWaveformIdentified) {
        return nullptr;
    }

    if (cue->isStreaming) {
        auto externalAwb = _externalAwb;

        if (externalAwb == nullptr) {
            return nullptr;
        }

        auto &files = externalAwb->GetFiles();
        if (files.find(cue->waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(cue->waveformId);

        return &file;
    } else {
        auto internalAwb = _internalAwb;

        if (internalAwb == nullptr) {
            return nullptr;
        }

        auto &files = internalAwb->GetFiles();

        if (files.find(cue->waveformId) == files.end()) {
            return nullptr;
        }

        auto &file = files.at(cue->waveformId);

        return &file;
    }
}

auto CAcbFile::ChooseSourceStream(const ACB_CUE_RECORD *cue) const -> IStream * {
    if (cue == nullptr) {
        return nullptr;
    }

    if (cue->isStreaming) {
        return _externalAwb->GetStream();
    } else {
        return this->GetStream();
    }
}

static auto GetExtensionForEncodeType(std::uint8_t encodeType) -> std::string {
    auto type = static_cast<CGSS_ACB_WAVEFORM_ENCODE_TYPE>(encodeType);

    switch (type) {
    case CGSS_ACB_WAVEFORM_ADX:
        return ".adx";
    case CGSS_ACB_WAVEFORM_HCA:
    case CGSS_ACB_WAVEFORM_HCA2:
        return ".hca";
    case CGSS_ACB_WAVEFORM_VAG:
        return ".vag";
    case CGSS_ACB_WAVEFORM_ATRAC3:
        return ".at3";
    case CGSS_ACB_WAVEFORM_BCWAV:
        return ".bcwav";
    case CGSS_ACB_WAVEFORM_NINTENDO_DSP:
        return ".dsp";
    default:
        break;
    }

    return std::format(".et-{:d}", encodeType);
}

template<typename T>
auto GetFieldValueAsNumber(
    CUtfTable *table, std::uint32_t rowIndex, const char *fieldName, T *result
) -> bool_t requires(std::is_arithmetic_v<T>)
{
    if (result) {
        std::memset(result, 0, sizeof(T));
    }

    auto &rows = table->GetRows();

    if (rowIndex >= rows.size()) {
        return FALSE;
    }

    auto &row = rows[rowIndex];

    for (auto &field : row.fields) {
        if (std::strcmp(fieldName, field->name) == 0) {
            if (result) {
                switch (field->type) {
                case CGSS_UTF_COLUMN_TYPE_U8:
                    *result = static_cast<T>(field->value.u8);
                    break;
                case CGSS_UTF_COLUMN_TYPE_S8:
                    *result = static_cast<T>(field->value.s8);
                    break;
                case CGSS_UTF_COLUMN_TYPE_U16:
                    *result = static_cast<T>(field->value.u16);
                    break;
                case CGSS_UTF_COLUMN_TYPE_S16:
                    *result = static_cast<T>(field->value.s16);
                    break;
                case CGSS_UTF_COLUMN_TYPE_U32:
                    *result = static_cast<T>(field->value.u32);
                    break;
                case CGSS_UTF_COLUMN_TYPE_S32:
                    *result = static_cast<T>(field->value.s32);
                    break;
                case CGSS_UTF_COLUMN_TYPE_U64:
                    *result = static_cast<T>(field->value.u64);
                    break;
                case CGSS_UTF_COLUMN_TYPE_S64:
                    *result = static_cast<T>(field->value.s64);
                    break;
                case CGSS_UTF_COLUMN_TYPE_R32:
                    *result = static_cast<T>(field->value.r32);
                    break;
                case CGSS_UTF_COLUMN_TYPE_R64:
                    *result = static_cast<T>(field->value.r64);
                    break;
                default:
                    throw CInvalidOperationException(
                        "Unsupported field type for retrieving numeric value."
                    );
                }
            }

            return TRUE;
        }
    }

    return FALSE;
}

auto GetFieldValueAsString(
    CUtfTable *table, std::uint32_t rowIndex, const char *fieldName, std::string &s
) -> bool_t {
    auto &rows = table->GetRows();

    if (rowIndex >= rows.size()) {
        return FALSE;
    }

    auto &row = rows[rowIndex];

    for (auto &field : row.fields) {
        if (std::strcmp(fieldName, field->name) == 0) {
            s = std::string(field->value.str);

            return TRUE;
        }
    }

    return FALSE;
}

CGSS_NS_END
