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

class CAcbFile final: public CUtfTable {

    __extends(CUtfTable, CAcbFile);

public:
    CGSS_EXPORT CAcbFile(IStream *stream, const char *fileName);

    CGSS_EXPORT CAcbFile(IStream *stream, std::uint64_t streamOffset, const char *fileName);

    CGSS_EXPORT ~CAcbFile() override;

    CGSS_EXPORT auto GetFileNames() const -> const std::vector<std::string> &;

    CGSS_EXPORT auto GetFileName() const -> const char *;

    CGSS_EXPORT auto
    OpenDataStream(const AFS2_FILE_RECORD *fileRecord, bool_t isStreaming) const -> IStream *;

    CGSS_EXPORT auto OpenDataStream(const char *fileName) const -> IStream *;

    CGSS_EXPORT auto OpenDataStream(std::uint32_t cueId) const -> IStream *;

    CGSS_EXPORT auto GetSymbolicFileNameHintByCueId(std::uint32_t cueId) const -> std::string;

    CGSS_EXPORT static auto GetSymbolicFileBaseNameByCueId(std::uint32_t cueId) -> std::string;

    CGSS_EXPORT auto GetSymbolicFileNameHintByTrackIndex(std::uint32_t trackIndex
    ) const -> std::string;

    CGSS_EXPORT static auto GetSymbolicFileBaseNameByTrackIndex(std::uint32_t trackIndex
    ) -> std::string;

    CGSS_EXPORT auto GetCueNameByCueId(std::uint32_t cueId) const -> std::string;

    CGSS_EXPORT auto GetCueNameByTrackIndex(std::uint32_t trackIndex) const -> std::string;

    CGSS_EXPORT auto GetCueRecordByWaveformFileName(const char *waveformFileName
    ) const -> const ACB_CUE_RECORD *;

    CGSS_EXPORT auto GetCueRecordByCueId(std::uint32_t cueId) const -> const ACB_CUE_RECORD *;

    CGSS_EXPORT auto GetFileRecordByWaveformFileName(const char *waveformFileName
    ) const -> const AFS2_FILE_RECORD *;

    CGSS_EXPORT auto GetFileRecordByCueId(std::uint32_t cueId) const -> const AFS2_FILE_RECORD *;

    CGSS_EXPORT auto GetFileRecordByTrackIndex(std::uint32_t trackIndex
    ) const -> const AFS2_FILE_RECORD *;

    CGSS_EXPORT auto GetTrackCountOfCueByCueId(std::uint32_t cueId) const -> std::uint32_t;

    CGSS_EXPORT auto GetTrackIndicesOfCueByCueId(
        std::uint32_t cueId, std::uint32_t *numberOfTracks, std::uint32_t *trackIndices
    ) const -> bool_t;

    CGSS_EXPORT auto GetTrackIndicesOfCueByCueId(
        std::uint32_t cueId, std::vector<std::uint32_t> &trackIndices
    ) const -> bool_t;

    CGSS_EXPORT auto GetTrackRecords() const -> const std::vector<ACB_TRACK_RECORD> &;

    CGSS_EXPORT auto IsCueIdentified(std::uint32_t cueId) const -> bool_t;

    CGSS_EXPORT void Initialize();

    CGSS_EXPORT auto GetInternalAwb() const -> const CAfs2Archive *;

    CGSS_EXPORT auto GetExternalAwb() const -> const CAfs2Archive *;

    CGSS_EXPORT auto GetFormatVersion() const -> std::uint32_t;

    CGSS_EXPORT auto GetFileExtensionHintByCueId(std::uint32_t cueId) const -> std::string;

    CGSS_EXPORT auto GetFileExtensionHintByWaveformFileName(const char *waveformFileName
    ) const -> std::string;

    CGSS_EXPORT auto GetFileExtensionHintByTrackIndex(std::uint32_t trackIndex
    ) const -> std::string;

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
