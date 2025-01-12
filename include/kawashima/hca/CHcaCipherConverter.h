#ifndef ACB_KAWASHIMA_HCA_CHCACIPHERCONVERTER_H_
#define ACB_KAWASHIMA_HCA_CHCACIPHERCONVERTER_H_

#include <cstdint>
#include <map>

#include "acb_cdata.h"
#include "acb_env.h"
#include "acb_env_ns.h"

#include "./CHcaFormatReader.h"

ACB_NS_BEGIN

class CHcaCipher;

class CHcaCipherConverter: public CHcaFormatReader {

    _extends(CHcaFormatReader, CHcaFormatReader);

public:
    /**
     * Creates a new HCA cipher converter.
     * @param stream Source stream.
     * @param cryptFrom Source cipher type.
     * @param cryptTo Wanted cipher type.
     * @return
     */
    ACB_EXPORT CHcaCipherConverter(
        IStream *stream, const HCA_CIPHER_CONFIG &cryptFrom, const HCA_CIPHER_CONFIG &cryptTo
    );

    CHcaCipherConverter(const CHcaCipherConverter &) = delete;

    CHcaCipherConverter(CHcaCipherConverter &&) = delete;

    auto operator=(const CHcaCipherConverter &) -> CHcaCipherConverter & = delete;

    auto operator=(CHcaCipherConverter &&) -> CHcaCipherConverter & = delete;

    ACB_EXPORT ~CHcaCipherConverter() override;

    ACB_EXPORT auto Read(
        void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
    ) -> std::size_t override;

    ACB_EXPORT auto GetPosition() -> std::uint64_t override;

    ACB_EXPORT void SetPosition(std::uint64_t value) override;

    ACB_EXPORT auto GetLength() -> std::uint64_t override;

private:
    auto ConvertBlock(std::uint32_t blockIndex) -> const std::uint8_t *;

    auto ConvertHeader() -> const std::uint8_t *;

    void InitializeExtra();

    CHcaCipher *_cipherFrom = nullptr;
    CHcaCipher *_cipherTo   = nullptr;
    HCA_CIPHER_CONFIG _ccFrom, _ccTo;
    std::uint8_t *_headerBuffer;
    std::map<std::uint32_t, const std::uint8_t *> _blockBuffers;
    std::uint64_t _position;
};

ACB_NS_END

#endif // ACB_KAWASHIMA_HCA_CHCACIPHERCONVERTER_H_
