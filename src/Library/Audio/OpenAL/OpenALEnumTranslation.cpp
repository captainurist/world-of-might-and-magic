#include "OpenALEnumTranslation.h"

#include <AL/alext.h>

#include <cassert>

ALenum translatePcmFormat(PcmBitness bitness, PcmChannels channels) {
    if (bitness == PCM_8BIT) {
        switch (channels) {
        default:            assert(false); [[fallthrough]];
        case PCM_MONO:      return AL_FORMAT_MONO8;
        case PCM_STEREO:    return AL_FORMAT_STEREO8;
        case PCM_4CH:       return AL_FORMAT_QUAD8;
        case PCM_51CH:      return AL_FORMAT_51CHN8;
        case PCM_61CH:      return AL_FORMAT_61CHN8;
        case PCM_71CH:      return AL_FORMAT_71CHN8;
        }
    } else {
        assert(bitness == PCM_16BIT);
        switch (channels) {
        default:            assert(false); [[fallthrough]];
        case PCM_MONO:      return AL_FORMAT_MONO16;
        case PCM_STEREO:    return AL_FORMAT_STEREO16;
        case PCM_4CH:       return AL_FORMAT_QUAD16;
        case PCM_51CH:      return AL_FORMAT_51CHN16;
        case PCM_61CH:      return AL_FORMAT_61CHN16;
        case PCM_71CH:      return AL_FORMAT_71CHN16;
        }
    }
}

AudioState translateOpenALSourceState(ALenum state) {
    switch (state) {
    default:            assert(false); [[fallthrough]];
    case AL_INITIAL:
    case AL_STOPPED:    return AUDIO_STOPPED;
    case AL_PLAYING:    return AUDIO_PLAYING;
    case AL_PAUSED:     return AUDIO_PAUSED;
    }
}
