#include "OpenALSource.h"

#include <cassert>
#include <utility>
#include <algorithm>

#include "OpenALSharedState.h"
#include "OpenALBuffer.h"
#include "OpenALEnumTranslation.h"

static constexpr size_t MIN_PENDING_BUFFERS = 16;

template<class T>
static void clear(std::queue<T> &queue) {
    std::queue<T> dummy;
    queue.swap(dummy);
}

OpenALSource::OpenALSource(OpenALSharedState *state, ALuint sourceId) : _state(state), _sourceId(sourceId) {
    assert(state);
    assert(sourceId != 0); // 0 is a null buffer id, not a source id.

    _state->registerSource(this);
}

OpenALSource::~OpenALSource() {
    _state->assertContext();

    alDeleteSources(1, &_sourceId);
    _state->maybeLogOpenALError("alDeleteSources");
    _state->unregisterSource(this);
}

Vec3f OpenALSource::position() const {
    _state->assertContext();

    Vec3f result;
    alGetSource3f(_sourceId, AL_POSITION, &result.x, &result.y, &result.z);
    _state->maybeLogOpenALError("alGetSource3f");
    return result;
}

void OpenALSource::setPosition(Vec3f position) {
    _state->assertContext();

    alSource3f(_sourceId, AL_POSITION, position.x, position.y, position.z);
    _state->maybeLogOpenALError("alSource3f");
}

bool OpenALSource::isLooping() const {
    _state->assertContext();

    ALint result = AL_FALSE;
    alGetSourcei(_sourceId, AL_LOOPING, &result);
    _state->maybeLogOpenALError("alGetSourcei");
    return result == AL_TRUE;
}

void OpenALSource::setLooping(bool looping) {
    _state->assertContext();

    alSourcei(_sourceId, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
    _state->maybeLogOpenALError("alSourcei");
}

float OpenALSource::volume() const {
    _state->assertContext();

    float result = 0;
    alGetSourcef(_sourceId, AL_GAIN, &result);
    _state->maybeLogOpenALError("alGetSourcef");
    return result;
}

void OpenALSource::setVolume(float volume) {
    _state->assertContext();

    alSourcef(_sourceId, AL_GAIN, std::clamp(volume, 0.0f, 1.0f));
    _state->maybeLogOpenALError("alSourcef");
}

AudioState OpenALSource::state() const {
    _state->assertContext();

    ALint alState = AL_STOPPED;
    alGetSourcei(_sourceId, AL_SOURCE_STATE, &alState);
    if (ALenum errorCode = alGetError(); errorCode != AL_NO_ERROR) {
        _state->logOpenALError("alGetSourcei", errorCode);
        return AUDIO_STOPPED; // Sane default, but not 100% correct.
    }
    return translateOpenALSourceState(alState);
}

void OpenALSource::setState(AudioState state) {
    _state->assertContext();

    // We want sane state handling logic, not what OpenAL does. The problematic function is alSourcePlay, which
    // rewinds if called on a source that's already playing. Thus, we need to query the current state first.
    ALint alState = AL_STOPPED;
    alGetSourcei(_sourceId, AL_SOURCE_STATE, &alState);
    if (ALenum errorCode = alGetError(); errorCode != AL_NO_ERROR) {
        _state->logOpenALError("alGetSourcei", errorCode);
        // In this case we want to call into OpenAL unconditionally.
    } else {
        AudioState currentState = translateOpenALSourceState(alState);

        // State didn't change => nothing to do.
        if (state == currentState)
            return;

        // There is no AUDIO_STOPPED -> AUDIO_PAUSED transition in the FSM, but we don't warn in this case b/c the
        // call is perfectly valid. E.g. imagine a source playing something, running out of data, transitioning into
        // AUDIO_STOPPED, and the user then calling setState(AUDIO_PAUSED). Note that checking state() first won't work
        // because the source might run out of data between the state() and setState() calls.
        if (state == AUDIO_PAUSED && currentState == AUDIO_STOPPED)
            return;
    }

    switch (state) {
    default:
        assert(false);
        [[fallthrough]];
    case AUDIO_STOPPED:
        alSourceStop(_sourceId);
        _state->maybeLogOpenALError("alSourceStop");
        break;
    case AUDIO_PLAYING:
        alSourcePlay(_sourceId);
        _state->maybeLogOpenALError("alSourcePlay");
        break;
    case AUDIO_PAUSED:
        alSourcePause(_sourceId);
        _state->maybeLogOpenALError("alSourcePause");
        break;
    }
}

void OpenALSource::enqueueBuffer(std::shared_ptr<AudioBuffer> buffer) {
    assert(buffer);
    assert(dynamic_cast<OpenALBuffer *>(buffer.get()));
    _state->assertContext();

    // Dequeue first.
    ALint processed = 0;
    alGetSourcei(_sourceId, AL_BUFFERS_PROCESSED, &processed);
    _state->maybeLogOpenALError("alGetSourcei");

    if (processed) {
        if (_tmp.size() < processed)
            _tmp.resize(processed);
        alSourceUnqueueBuffers(_sourceId, processed, _tmp.data());
        _state->maybeLogOpenALError("alSourceUnqueueBuffers");

        assert(_buffers.size() >= processed);
        for (int i = 0; i < processed; i++)
            _buffers.pop();
    }

    // Then push the new buffer.
    ALuint bufferId = static_cast<OpenALBuffer *>(buffer.get())->bufferId(); // Dynamic type is checked in an assert above.
    alSourceQueueBuffers(_sourceId, 1, &bufferId);
    if (ALenum errorCode = alGetError(); errorCode != AL_NO_ERROR) {
        _state->logOpenALError("alSourceQueueBuffers", errorCode);
    } else {
        _buffers.push(std::move(buffer));
    }
}

void OpenALSource::clearBuffers() {
    _state->assertContext();

    // TODO(captainurist): do we need to stop first?

    // Detach all buffers.
    alSourcei(_sourceId, AL_BUFFER, 0);
    _state->maybeLogOpenALError("alSourcei");

    // Clear state.
    clear(_buffers);
}

int OpenALSource::queuedBufferCount() {
    ALint queued = 0;
    alGetSourcei(_sourceId, AL_BUFFERS_QUEUED, &queued);
    _state->maybeLogOpenALError("alGetSourcei");
    return queued;
}
