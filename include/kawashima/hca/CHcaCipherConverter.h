#ifndef CGSS_KAWASHIMA_HCA_CHCACIPHERCONVERTER_H_
#define CGSS_KAWASHIMA_HCA_CHCACIPHERCONVERTER_H_

#include <cstdint>
#include <map>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"

#include "./CHcaFormatReader.h"

CGSS_NS_BEGIN

class CHcaCipher;

class CGSS_EXPORT CHcaCipherConverter: public CHcaFormatReader {

    __extends(CHcaFormatReader, CHcaFormatReader);

public:
    /**
     * Creates a new HCA cipher converter.
     * @param stream Source stream.
     * @param cryptFrom Source cipher type.
     * @param cryptTo Wanted cipher type.
     * @return
     */
    CHcaCipherConverter(
        IStream *stream, const HCA_CIPHER_CONFIG &cryptFrom, const HCA_CIPHER_CONFIG &cryptTo
    );

    CHcaCipherConverter(const CHcaCipherConverter &) = delete;

    CHcaCipherConverter(CHcaCipherConverter &&) = delete;

    auto operator=(const CHcaCipherConverter &) -> CHcaCipherConverter & = delete;

    auto operator=(CHcaCipherConverter &&) -> CHcaCipherConverter & = delete;

    ~CHcaCipherConverter() override;

    auto Read(void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count)
        -> std::uint32_t override;

    auto GetPosition() -> std::uint64_t override;

    void SetPosition(std::uint64_t value) override;

    auto GetLength() -> std::uint64_t override;

private:
    auto ConvertBlock(std::uint32_t blockIndex) -> const std::uint8_t *;

    auto ConvertHeader() -> const std::uint8_t *;

    void InitializeExtra();

    CHcaCipher *_cipherFrom, *_cipherTo;
    HCA_CIPHER_CONFIG _ccFrom, _ccTo;
    std::uint8_t *_headerBuffer;
    std::map<std::uint32_t, const std::uint8_t *> _blockBuffers;
    std::uint64_t _position;
};

CGSS_NS_END

#endif // CGSS_KAWASHIMA_HCA_CHCACIPHERCONVERTER_H_
