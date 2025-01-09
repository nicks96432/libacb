#ifndef CGSS_KAWASHIMA_HCA_CHCAFORMATREADER_H_
#define CGSS_KAWASHIMA_HCA_CHCAFORMATREADER_H_

#include <array>
#include <cstdint>

#include "cgss_cdata.h"
#include "cgss_env.h"
#include "cgss_env_ns.h"
#include "takamori/streams/CStream.h"

CGSS_NS_BEGIN

class CHcaFormatReader: public CStream {

    __extends(CStream, CHcaFormatReader);

public:
    CGSS_EXPORT explicit CHcaFormatReader(IStream *baseStream);

    CHcaFormatReader(const CHcaFormatReader &) = delete;

    ~CHcaFormatReader() override = default;

    /**
     * Retrieves the HCA meta information.
     * @param info Retrieved HCA information.
     * @return
     */
    CGSS_EXPORT void GetHcaInfo(HCA_INFO &info) const;

    CGSS_EXPORT void GetHcaInfo(HCA_INFO *pInfo) const;

    [[nodiscard]] CGSS_EXPORT auto GetHcaInfo() const -> const HCA_INFO &;

    CGSS_EXPORT auto Write(
        const void *buffer, std::size_t bufferSize, std::size_t offset, std::size_t count
    ) -> std::size_t final;

    [[nodiscard]] CGSS_EXPORT auto IsWritable() const -> bool_t final;

    [[nodiscard]] CGSS_EXPORT auto IsReadable() const -> bool_t final;

    [[nodiscard]] CGSS_EXPORT auto IsSeekable() const -> bool_t final;

    CGSS_EXPORT void SetLength(std::uint64_t value) final;

    CGSS_EXPORT void Flush() final;

    CGSS_EXPORT static auto IsPossibleHcaStream(IStream *stream) -> bool_t;

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

CGSS_NS_END

#endif // CGSS_KAWASHIMA_HCA_CHCAFORMATREADER_H_
