#pragma once

#include <memory>
#include <cstdint>
#include <span>

#include "Utility/Workaround/ToUnderlying.h"
#include "Utility/Memory/FreeDeleter.h"

#include "PcmEnums.h"

/**
 * Like `Image`, but for audio, and unlike `Image` doesn't lift the data type into a template parameter.
 */
class PcmData {
 public:
    PcmData() = default;
    PcmData(const PcmData &) = delete; // Non-copyable.
    PcmData(PcmData &&) = default; // Movable.
    PcmData &operator=(const PcmData &) = delete; // Non-copyable.
    PcmData &operator=(PcmData &&) = default; // Movable.

    static PcmData uninitialized(PcmBitness bitness, PcmChannels channels, int frequency, int samples);

    [[nodiscard]] PcmBitness bitness() const {
        return _bitness;
    }

    [[nodiscard]] PcmChannels channels() const {
        return _channels;
    }

    [[nodiscard]] int frequency() const {
        return _frequency;
    }

    [[nodiscard]] int samples() const {
        return _samples;
    }

    [[nodiscard]] std::span<uint8_t> data() {
        return {static_cast<uint8_t *>(_data.get()), calculateSize(_bitness, _channels, _samples)};
    }

    [[nodiscard]] std::span<const uint8_t> data() const {
        return const_cast<PcmData &>(*this).data();
    }

    explicit operator bool() const {
        return static_cast<bool>(_data.get());
    }

 private:
    static size_t calculateSize(PcmBitness bitness, PcmChannels channels, int samples) {
        return std::to_underlying(bitness) / 8 * std::to_underlying(channels) * samples;
    };

 private:
    PcmBitness _bitness = PCM_8BIT;
    PcmChannels _channels = PCM_MONO;
    int _frequency = 0;
    int _samples = 0;
    std::unique_ptr<void, FreeDeleter> _data;
};
