#include "PcmData.h"

#include <cassert>

PcmData PcmData::uninitialized(PcmBitness bitness, PcmChannels channels, int frequency, int samples) {
    assert(frequency > 0);
    assert(samples >= 0);

    if (samples == 0)
        return {};

    PcmData result;
    result._bitness = bitness;
    result._channels = channels;
    result._frequency = frequency;
    result._samples = samples;
    result._data.reset(malloc(calculateSize(bitness, channels, samples)));
    return result;
}
