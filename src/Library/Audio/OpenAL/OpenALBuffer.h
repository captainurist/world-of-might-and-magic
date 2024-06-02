#pragma once

#include <AL/al.h>

#include "Library/Audio/Interface/AudioBuffer.h"

class OpenALSharedState;

class OpenALBuffer : public AudioBuffer {
 public:
    OpenALBuffer(OpenALSharedState *state, ALuint bufferId);
    virtual ~OpenALBuffer();

    ALuint bufferId() const {
        return _bufferId;
    }

 private:
    OpenALSharedState *_state = nullptr;
    ALuint _bufferId = 0;
};
