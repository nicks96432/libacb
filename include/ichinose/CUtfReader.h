#ifndef ACB_ICHINOSE_CUTFREADER_H_
#define ACB_ICHINOSE_CUTFREADER_H_

#include <cstddef>
#include <cstdint>

#include "acb_env.h"
#include "acb_env_ns.h"
#include "takamori/streams/IStream.h"

ACB_NS_BEGIN

class ACB_EXPORT CUtfReader final {

    __root_class(CUtfReader);

public:
    CUtfReader();

    CUtfReader(std::uint8_t seed, std::uint8_t increment);

    [[nodiscard]] auto IsEncrypted() const -> bool_t;

    void PeekBytes(
        IStream *stream,
        std::uint8_t *buffer,
        std::size_t streamOffset,
        std::size_t size,
        std::size_t utfOffset
    );

    void PeekBytes(
        IStream *stream,
        std::uint8_t *buffer,
        std::size_t bufferOffset,
        std::size_t streamOffset,
        std::size_t size,
        std::size_t utfOffset
    );

    auto
    PeekUInt8(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> std::uint8_t;

    auto PeekInt8(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> std::int8_t;

    auto
    PeekUInt16(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> std::uint16_t;

    auto
    PeekInt16(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> std::int16_t;

    auto
    PeekUInt32(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> std::uint32_t;

    auto
    PeekInt32(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> std::int32_t;

    auto
    PeekUInt64(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> std::uint64_t;

    auto
    PeekInt64(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> std::int64_t;

    auto PeekSingle(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> float;

    auto PeekDouble(IStream *stream, std::size_t streamOffset, std::size_t utfOffset) -> double;

private:
    const bool_t _encrypted;
    const std::uint8_t _seed;
    const std::uint8_t _increment;

    std::uint8_t _currentXor;
    std::uint64_t _currentUtfOffset;
    std::uint8_t _currentStringXor;
    std::uint64_t _currentUtfStringOffset;
};

ACB_NS_END

#endif // ACB_ICHINOSE_CUTFREADER_H_
