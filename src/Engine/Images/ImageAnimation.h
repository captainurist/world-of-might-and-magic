#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

#include "ImageId.h"

/**
 * This struct presents an abstraction of an animated image.
 *
 * Such images aren't used that much in M&M, but some examples are:
 * - Cleric painting in Haunted Mansion in M&M7. His eyes glow red.
 * - Ghost of Balthazar in M&M6.
 */
class ImageAnimation {
public:
    ImageAnimation(int duration, std::vector<int> timestamps, std::vector<ImageId> frames) {
        assert(duration > 0);
        assert(!timestamps.empty());
        assert(timestamps.size() == frames.size());
        assert(timestamps.back() == duration);
        assert(std::is_sorted(timestamps.begin(), timestamps.end()));

        _duration = duration;
        _timestamps = std::move(timestamps);
        _frames = std::move(frames);
    }

    int duration() const {
        return _duration;
    }

    const std::vector<int> &timestamps() const {
        return _timestamps;
    }

    const std::vector<ImageId> &frames() const {
        return _frames;
    }

    // should belong to a materialized class w/texture handles
#if 0
    /**
     * @param time                      Current real time, in 1/128th of a second.
     * @return                          Image id to use.
     */
    const ImageId &frame(int time) const {
        time = time % _duration;

        auto pos = std::lower_bound(_frames.begin(), _frames.end(), time, [](const ImageAnimationFrame &frame, int time) {
            return frame.endTime < time;
        });
        assert(pos < _frames.end());
        return pos->imageId;
    }
#endif

private:
    int _duration = 0; // Total duration, in 1/128th of a real-time second.
    std::vector<int> _timestamps; // Frame end timestamps.
    std::vector<ImageId> _frames;
};
