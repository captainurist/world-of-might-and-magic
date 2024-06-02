#pragma once

// Note that std::to_underlying gives exactly the values one would expect for the enums here.

enum class PcmBitness {
    PCM_8BIT = 8,
    PCM_16BIT = 16
};
using enum PcmBitness;

enum class PcmChannels {
    PCM_MONO = 1,
    PCM_STEREO = 2,
    PCM_4CH = 4,
    PCM_51CH = 6,
    PCM_61CH = 7,
    PCM_71CH = 8
};
using enum PcmChannels;
