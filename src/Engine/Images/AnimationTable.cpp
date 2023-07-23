#include "AnimationTable.h"

#include "Engine/Serialization/LegacyImages.h"

#include "AnimationTableEntry.h"

void AnimationTable::loadFromFile(const Blob &mm7data) {
    std::vector<AnimationTableEntry> entries;

    deserialize(mm7data, &entries, tags::append, tags::via<TextureFrame_MM7>);
}

void AnimationTable::importAnimations(const std::vector<AnimationTableEntry> &entries) {
    std::string name;
    int duration = 0;
    std::vector<int> timestamps;
    std::vector<ImageId> frames;

    for (const AnimationTableEntry &entry : entries) {
        if (entry.flags & ANIMATION_FIRST_FRAME) {
            name = entry.name;
            duration = entry.totalDuration;
            frames.clear();
            timestamps.clear();
        }

        timestamps.push_back((timestamps.empty() ? 0 : timestamps.back()) + entry.frameDuration);
        frames.push_back(ImageId::regular(entry.name));

        if (!(entry.flags & ANIMATION_MORE_FRAMES) && duration > 0) // If duration is zero then it's a dummy, no need to store it.
            _animationByName.emplace(name, ImageAnimation(duration, std::move(timestamps), std::move(frames)));
    }
}
