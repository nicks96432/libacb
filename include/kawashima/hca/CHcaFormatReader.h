#ifndef ACB_KAWASHIMA_HCA_CHCAFORMATREADER_H_
#define ACB_KAWASHIMA_HCA_CHCAFORMATREADER_H_

#include <array>
#include <cstdint>

#include "acb_cdata.h"
#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/streams/CStream.h"

ACB_NS_BEGIN

class CHcaFormatReader: public CStream {

    _extends(CStream, CHcaFormatReader);

public:
    ACB_EXPORT explicit CHcaFormatReader(IStream *baseStream);

    CHcaFormatReader(const CHcaFormatReader &) = delete;

    ~CHcaFormatReader() override = default;

    /**
     * Retrieves the HCA meta information.
     * @param info Retrieved HCA information.
     * @return
     */
    ACB_EXPORT void GetHcaInfo(HCA_INFO &info) const;

    ACB_EXPORT void GetHcaInfo(HCA_INFO *pInfo) const;

    [[nodiscard]] ACB_EXPORT auto GetHcaInfo() const -> const HCA_INFO &;

    ACB_EXPORT auto Write(
        const void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
    ) -> std::size_t final;

    [[nodiscard]] ACB_EXPORT auto IsWritable() const -> bool_t final;

    [[nodiscard]] ACB_EXPORT auto IsReadable() const -> bool_t final;

    [[nodiscard]] ACB_EXPORT auto IsSeekable() const -> bool_t final;

    ACB_EXPORT void SetLength(std::uint64_t value) final;

    ACB_EXPORT void Flush() final;

    ACB_EXPORT static auto IsPossibleHcaStream(IStream *stream) -> bool_t;

protected:
    static auto
    ComputeChecksum(void *pData, std::uint32_t dwDataSize, std::uint16_t wInitSum) -> std::uint16_t;

    HCA_INFO _hcaInfo;

    IStream *_baseStream;

private:
    void Initialize();

    void PrintHcaInfo();

    static const std::array<std::uint16_t, 256> ChecksumTable;
};

ACB_NS_END

#endif // ACB_KAWASHIMA_HCA_CHCAFORMATREADER_H_
