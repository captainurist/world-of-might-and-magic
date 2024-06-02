#pragma once

#include <variant>
#include <utility>
#include <chrono>

#include "Library/Pcm/PcmData.h"
#include "Library/Image/Image.h"

#include "MediaEnums.h"

class MediaPacket {
 public:
    MediaPacket() : _data(nullptr) {}
    MediaPacket(PcmData audio) : _data(std::move(audio)) {}
    MediaPacket(RgbaImage video) : _data(std::move(video)) {}

    std::chrono::microseconds timestamp() const {
        return _timestamp;
    }

    MediaType type() const;

    const PcmData &audio() const {
        return std::get<PcmData>(_data);
    }

    const RgbaImage &video() const {
        return std::get<RgbaImage>(_data);
    }

    explicit operator bool() const {
        return !std::holds_alternative<std::nullptr_t>(_data);
    }

 private:
    std::chrono::microseconds _timestamp = std::chrono::microseconds(0);
    std::variant<std::nullptr_t, PcmData, RgbaImage> _data;
};
