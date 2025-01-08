#ifndef CGSS_ICHINOSE_CACBFILE_H_
#define CGSS_ICHINOSE_CACBFILE_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "ichinose/CAfs2Archive.h"

#include "./CUtfTable.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CAcbFile final: public CUtfTable {

    __extends(CUtfTable, CAcbFile);

public:
    CAcbFile(IStream *stream, const char *fileName);

    CAcbFile(IStream *stream, std::uint64_t streamOffset, const char *fileName);

    ~CAcbFile() override;

    auto GetFileNames() const -> const std::vector<std::string> &;

    auto GetFileName() const -> const char *;

    auto OpenDataStream(const AFS2_FILE_RECORD *fileRecord, bool_t isStreaming) const -> IStream *;

    auto OpenDataStream(const char *fileName) const -> IStream *;

    auto OpenDataStream(std::uint32_t cueId) const -> IStream *;

    auto GetSymbolicFileNameHintByCueId(std::uint32_t cueId) const -> std::string;

    static auto GetSymbolicFileBaseNameByCueId(std::uint32_t cueId) -> std::string;

    auto GetSymbolicFileNameHintByTrackIndex(std::uint32_t trackIndex) const -> std::string;

    static auto GetSymbolicFileBaseNameByTrackIndex(std::uint32_t trackIndex) -> std::string;

    auto GetCueNameByCueId(std::uint32_t cueId) const -> std::string;

    auto GetCueNameByTrackIndex(std::uint32_t trackIndex) const -> std::string;

    auto GetCueRecordByWaveformFileName(const char *waveformFileName
    ) const -> const ACB_CUE_RECORD *;

    auto GetCueRecordByCueId(std::uint32_t cueId) const -> const ACB_CUE_RECORD *;

    auto GetFileRecordByWaveformFileName(const char *waveformFileName
    ) const -> const AFS2_FILE_RECORD *;

    auto GetFileRecordByCueId(std::uint32_t cueId) const -> const AFS2_FILE_RECORD *;

    auto GetFileRecordByTrackIndex(std::uint32_t trackIndex) const -> const AFS2_FILE_RECORD *;

    auto GetTrackCountOfCueByCueId(std::uint32_t cueId) const -> std::uint32_t;

    auto GetTrackIndicesOfCueByCueId(
        std::uint32_t cueId, std::uint32_t *numberOfTracks, std::uint32_t *trackIndices
    ) const -> bool_t;

    auto GetTrackIndicesOfCueByCueId(std::uint32_t cueId, std::vector<std::uint32_t> &trackIndices)
        const -> bool_t;

    auto GetTrackRecords() const -> const std::vector<ACB_TRACK_RECORD> &;

    auto IsCueIdentified(std::uint32_t cueId) const -> bool_t;

    void Initialize() override;

    auto GetInternalAwb() const -> const CAfs2Archive *;

    auto GetExternalAwb() const -> const CAfs2Archive *;

    auto GetFormatVersion() const -> std::uint32_t;

    auto GetFileExtensionHintByCueId(std::uint32_t cueId) const -> std::string;

    auto GetFileExtensionHintByWaveformFileName(const char *waveformFileName) const -> std::string;

    auto GetFileExtensionHintByTrackIndex(std::uint32_t trackIndex) const -> std::string;

    static constexpr std::uint32_t KEY_MODIFIER_ENABLED_VERSION = 0x01300000;

private:
    void InitializeCueList();

    void InitializeCueNameToWaveformMap();

    void InitializeTrackList();

    void InitializeAwbArchives();

    auto GetFileRecordByCueRecord(const ACB_CUE_RECORD *cue) const -> const AFS2_FILE_RECORD *;

    auto ChooseSourceStream(const ACB_CUE_RECORD *cue) const -> IStream *;

    auto FindExternalAwbFileName() const -> std::string;

    /**
     * You do not need to manually delete the pointer retrieved.
     * @param tableName
     */
    auto GetTable(const char *tableName) const -> CUtfTable *;

    auto ResolveTable(const char *tableName) const -> CUtfTable *;

    auto GetTrackRecordByTrackIndex(std::uint32_t trackIndex) const -> const ACB_TRACK_RECORD *;

    const CAfs2Archive *_internalAwb;
    const CAfs2Archive *_externalAwb;

    std::vector<std::string> _fileNames;
    std::vector<ACB_CUE_RECORD> _cues;
    std::vector<ACB_TRACK_RECORD> _tracks;

    std::map<std::string, std::uint16_t> _cueNameToWaveform;

    // TODO: Consider better design to follow constancy. (see GetTable(const char *))
    mutable std::map<std::string, CUtfTable *> _tables;

    std::uint32_t _formatVersion;

    const char *_fileName;
};

CGSS_NS_END

#endif // CGSS_ICHINOSE_CACBFILE_H_
