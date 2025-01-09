#ifndef CGSS_KAWASHIMA_HCA_CHCADECODER_H_
#define CGSS_KAWASHIMA_HCA_CHCADECODER_H_

#include <array>
#include <cstdint>
#include <map>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CHcaFormatReader.h"

CGSS_NS_BEGIN

class CHcaCipher;
class CHcaAth;
class CHcaChannel;

class CHcaDecoder: public CHcaFormatReader {

    __extends(CHcaFormatReader, CHcaDecoder);

public:
    CGSS_EXPORT explicit CHcaDecoder(IStream *stream);

    CGSS_EXPORT CHcaDecoder(IStream *stream, const HCA_DECODER_CONFIG &decoderConfig);

    CHcaDecoder(const CHcaDecoder &) = delete;

    CHcaDecoder(CHcaDecoder &&) = delete;

    auto operator=(const CHcaDecoder &) -> CHcaDecoder & = delete;

    auto operator=(CHcaDecoder &&) -> CHcaDecoder & = delete;

    CGSS_EXPORT ~CHcaDecoder() override;

    CGSS_EXPORT auto Read(
        void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
    ) -> std::size_t override;

    CGSS_EXPORT auto GetPosition() -> std::uint64_t override;

    CGSS_EXPORT void SetPosition(std::uint64_t value) override;

    CGSS_EXPORT auto GetLength() -> std::uint64_t override;

private:
    void InitializeExtra();

    /**
     * Generate a wave header for decoded file.
     * @remarks You can use GetWaveHeaderSize() to determine the header size before trying to get
     * wave header data.
     * @see ComputeWaveHeaderSize
     * @return
     */
    auto GenerateWaveHeader() -> const std::uint8_t *;

    /**
     * Computes the minimum size required for generated wave header.
     * @return Computed size.
     */
    auto GetWaveHeaderSize() -> std::uint32_t;

    /**
     * Continue to decode upcoming blocks, from HCA to wave audio, and write decoded data to data
     * buffer.
     * @remarks You can use ComputeWaveBlockSize() to determine the minimum size for the data buffer
     * before trying to decode. Decoded data are in blocks. This function will try to decode the
     * audio as much as possible when there is still data and the buffer space left is still enough
     * for a block.
     * @see ComputeWaveBlockSize
     * @return
     */
    auto DecodeBlock(std::uint32_t blockIndex) -> const std::uint8_t *;

    /**
     * Computes the minimum size required for decoded wave data block.
     * @return Computed size.
     */
    auto GetWaveBlockSize() -> std::uint32_t;

    /**
     * Map a linear position to a looped position, considering looping range.
     * @param linearPosition The wave stream position in linear order.
     * @return Mapped position. It falls in [0, waveHeaderSize + totalWaveBlocks * waveBlockSize].
     */
    auto MapLoopedPosition(std::uint64_t linearPosition) -> std::uint64_t;

    std::map<std::uint32_t, const std::uint8_t *> _decodedBlocks;

    static constexpr std::uint32_t ChannelCount = 0x10;

    CHcaAth *_ath;
    CHcaCipher *_cipher;
    HCA_DECODER_CONFIG _decoderConfig;
    std::array<CHcaChannel *, ChannelCount> _channels;
    std::uint32_t _waveHeaderSize;
    std::uint8_t *_waveHeaderBuffer;
    std::uint32_t _waveBlockSize;
    std::uint8_t *_hcaBlockBuffer;
    // Position measured by wave output.
    std::uint64_t _position;
};

CGSS_NS_END

#endif // CGSS_KAWASHIMA_HCA_CHCADECODER_H_
