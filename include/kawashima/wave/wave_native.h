#ifndef ACB_KAWASHIMA_WAVE_WAVE_NATIVE_H_
#define ACB_KAWASHIMA_WAVE_WAVE_NATIVE_H_

#include <cstdint>

#include "acb_env_ns.h"

ACB_NS_BEGIN

// NOLINTBEGIN(modernize-avoid-c-arrays)

struct WaveRiffSection {
    std::uint8_t riff[4];
    std::uint32_t riffSize;
    std::uint8_t wave[4];
    std::uint8_t fmt[4];
    std::uint32_t fmtSize;
    std::uint16_t fmtType;
    std::uint16_t fmtChannelCount;
    std::uint32_t fmtSamplingRate;
    std::uint32_t fmtSamplesPerSec;
    std::uint16_t fmtSamplingSize;
    std::uint16_t fmtBitCount;
};

struct WaveSampleSection {
    std::uint8_t smpl[4];
    std::uint32_t smplSize;
    std::uint32_t manufacturer;
    std::uint32_t product;
    std::uint32_t samplePeriod;
    std::uint32_t midiUnityNote;
    std::uint32_t midiPitchFraction;
    std::uint32_t smpteFormat;
    std::uint32_t smpteOffset;
    std::uint32_t sampleLoops;
    std::uint32_t samplerData;
    std::uint32_t loopIdentifier;
    std::uint32_t loopType;
    std::uint32_t loopStart;
    std::uint32_t loopEnd;
    std::uint32_t loopFraction;
    std::uint32_t loopPlayCount;
};

struct WaveNoteSection {
    std::uint8_t note[4];
    std::uint32_t noteSize;
    std::uint32_t dwName;
};

struct WaveDataSection {
    std::uint8_t data[4];
    std::uint32_t dataSize;
};

// NOLINTEND(modernize-avoid-c-arrays)

ACB_NS_END

#endif // ACB_KAWASHIMA_WAVE_WAVE_NATIVE_H_
