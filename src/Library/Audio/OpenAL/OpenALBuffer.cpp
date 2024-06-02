#include "OpenALBuffer.h"

#include <cassert>

#include "OpenALSharedState.h"

OpenALBuffer::OpenALBuffer(OpenALSharedState *state, ALuint bufferId) : _state(state), _bufferId(bufferId) {
    assert(state);
    assert(bufferId != 0);

    _state->registerBuffer(this);
}

OpenALBuffer::~OpenALBuffer() {
    _state->assertContext();

    alDeleteBuffers(1, &_bufferId);
    _state->maybeLogOpenALError("alDeleteBuffers");
    _state->unregisterBuffer(this);
}
