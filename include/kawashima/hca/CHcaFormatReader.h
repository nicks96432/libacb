#ifndef CGSS_KAWASHIMA_HCA_CHCAFORMATREADER_H_
#define CGSS_KAWASHIMA_HCA_CHCAFORMATREADER_H_

#include <cstdint>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "takamori/streams/CStream.h"

CGSS_NS_BEGIN

class CGSS_EXPORT CHcaFormatReader: public CStream {

    __extends(CStream, CHcaFormatReader);

public:
    explicit CHcaFormatReader(IStream *baseStream);

    CHcaFormatReader(const CHcaFormatReader &) = delete;

    ~CHcaFormatReader() override = default;

    /**
     * Retrieves the HCA meta information.
     * @param info Retrieved HCA information.
     * @return
     */
    void GetHcaInfo(HCA_INFO &info) const;

    void GetHcaInfo(HCA_INFO *pInfo) const;

    auto GetHcaInfo() const -> const HCA_INFO &;

    auto Write(
        const void *buffer, std::uint32_t bufferSize, std::size_t offset, std::uint32_t count
    ) -> std::uint32_t final;

    auto IsWritable() const -> bool_t final;

    auto IsReadable() const -> bool_t final;

    auto IsSeekable() const -> bool_t final;

    void SetLength(std::uint64_t value) final;

    void Flush() final;

    static auto IsPossibleHcaStream(IStream *stream) -> bool_t;

protected:
    static auto
    ComputeChecksum(void *pData, std::uint32_t dwDataSize, std::uint16_t wInitSum) -> std::uint16_t;

    HCA_INFO _hcaInfo;

    IStream *_baseStream;

private:
    void Initialize();

    void PrintHcaInfo();

    static const std::uint16_t *ChecksumTable;
};

CGSS_NS_END

#endif // CGSS_KAWASHIMA_HCA_CHCAFORMATREADER_H_
