#ifndef ACB_CDATA_H_
#define ACB_CDATA_H_

#include <cstddef>
#include <cstdint>

#include "acb_cenum.h"
#include "acb_env.h"

#pragma pack(push)
#pragma pack(1)

constexpr std::size_t ACB_CUE_RECORD_NAME_MAX_LEN = 256;

// NOLINTBEGIN(modernize-avoid-c-arrays)

struct ACB_CUE_RECORD {

    std::uint32_t cueId;
    std::uint8_t referenceType;
    std::uint16_t referenceIndex;

    bool_t isWaveformIdentified;
    std::uint16_t waveformIndex;
    std::uint16_t waveformId;
    std::uint8_t encodeType;
    bool_t isStreaming;

    char cueName[ACB_CUE_RECORD_NAME_MAX_LEN];
};

struct ACB_TRACK_RECORD {

    std::uint32_t trackIndex;
    std::uint16_t synthIndex;

    bool_t isWaveformIdentified;
    std::uint16_t waveformIndex;
    std::uint16_t waveformId;
    std::uint8_t encodeType;
    bool_t isStreaming;
};

constexpr std::size_t AFS2_FILE_NAME_MAX_LEN = 256;

struct AFS2_FILE_RECORD {

    std::uint16_t cueId;
    std::uint64_t fileOffsetRaw;
    std::uint64_t fileOffsetAligned;
    std::uint64_t fileSize;
    char fileName[AFS2_FILE_NAME_MAX_LEN];
};

struct HCA_CIPHER_CONFIG {

    union Key {
        struct KeyParts {
            std::uint32_t key2;
            std::uint32_t key1;
        } keyParts;

        std::uint64_t key;
    } key;

    ACB_HCA_CIPHER_TYPE cipherType;

    std::uint16_t keyModifier;
};

using HcaDecodeFunc = std::uint32_t (*)(float, std::uint8_t *, const std::uint32_t);

struct HCA_DECODER_CONFIG {

    HCA_CIPHER_CONFIG cipherConfig;
    bool_t waveHeaderEnabled;
    bool_t loopEnabled;
    std::uint32_t loopCount;
    HcaDecodeFunc decodeFunc;
};

struct HCA_INFO {
    /**
     * Major part of version number.
     */
    std::uint16_t versionMajor;
    /**
     * Minor part of version number.
     */
    std::uint16_t versionMinor;
    /**
     * Channel count.
     */
    std::uint32_t channelCount;
    /**
     * Sampling rate, in hertz.
     */
    std::uint32_t samplingRate;
    /**
     * Total number of data blocks.
     */
    std::uint32_t blockCount;
    /**
     * Size of each data block.
     */
    std::uint16_t blockSize;
    /**
     * ATH type.
     * Possible values (observed) are 0 and 1.
     */
    std::uint16_t athType;
    /**
     * Whether internal loop flag is on.
     */
    bool_t loopExists;
    /**
     * The block index at the start of looping segment.
     */
    std::uint32_t loopStart;
    /**
     * The block index at the end of looping segment.
     */
    std::uint32_t loopEnd;
    /**
     * Cipher type.
     */
    ACB_HCA_CIPHER_TYPE cipherType;
    /**
     * Relative volume in RVA (relative volume adjustment) table. It is the multiply factor for
     * floating point wave data.
     */
    float rvaVolume;
    /**
     * Length of comment.
     */
    std::uint8_t commentLength;
    /**
     * Comment data.
     */
    char comment[0x100];
    std::uint16_t fmtR01, fmtR02;
    std::uint16_t compR01, compR02, compR03, compR04, compR05, compR06, compR07, compR08;
    std::uint32_t compR09;
    std::uint16_t vbrR01, vbrR02;
    std::uint16_t loopR01, loopR02;
    /**
     * Offset of audio data. It usually equals to the size of HCA header.
     */
    std::uint32_t dataOffset;
};

constexpr std::size_t UTF_FIELD_MAX_NAME_LEN = 1024;

struct UTF_FIELD {

    ACB_UTF_COLUMN_TYPE type;
    ACB_UTF_COLUMN_STORAGE storage;

    std::uint32_t offset;
    std::uint32_t offsetInRow;

    char name[UTF_FIELD_MAX_NAME_LEN];

    union {
        std::uint8_t u8;
        std::int8_t s8;
        std::uint16_t u16;
        std::int16_t s16;
        std::uint32_t u32;
        std::int32_t s32;
        std::uint64_t u64;
        std::int64_t s64;
        float r32;
        double r64;

        struct {
            std::uint8_t *ptr;
            std::uint32_t size;
        } data;

        char *str;
    } value;
};

struct UTF_HEADER {

    std::uint32_t tableSize;
    std::uint16_t unk1;
    std::uint32_t perRowDataOffset;
    std::uint32_t stringTableOffset;
    std::uint32_t extraDataOffset;
    std::uint32_t tableNameOffset;
    std::uint16_t fieldCount;
    std::uint16_t rowSize;
    std::uint32_t rowCount;
};

// Used for viewing only. Alternative is CUtfTable::UtfRow.
struct UTF_ROW {

    std::uint32_t baseOffset;

    struct UTF_FIELD *fields;
};

constexpr std::size_t UTF_TABLE_MAX_NAME_LEN = 1024;

// A C structure used to present CUtfTable.
struct UTF_TABLE {

    struct UTF_HEADER header;

    struct UTF_ROW *rows;

    bool_t isEncrypted;

    char tableName[UTF_TABLE_MAX_NAME_LEN];
};

// NOLINTEND(modernize-avoid-c-arrays)

#pragma pack(pop)

#endif // ACB_CDATA_H
