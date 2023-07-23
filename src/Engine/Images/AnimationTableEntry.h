#pragma once

#include <string>

#include "Utility/Flags.h"

enum class AnimationTableEntryFlag {
    ANIMATION_MORE_FRAMES = 0x1,
    ANIMATION_FIRST_FRAME = 0x2,
};
using enum AnimationTableEntryFlag;
MM_DECLARE_FLAGS(AnimationTableEntryFlags, AnimationTableEntryFlag)
MM_DECLARE_OPERATORS_FOR_FLAGS(AnimationTableEntryFlags)


struct AnimationTableEntry {
    std::string name;
    int totalDuration = 0; // Total animation duration, in 1/128th of a real-time second.
    int frameDuration = 0; // Frame duration, in 1/128th of a real-time second.
    AnimationTableEntryFlags flags;
};
