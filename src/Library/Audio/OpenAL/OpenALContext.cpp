#include "OpenALContext.h"

#include <cassert>

#include "OpenALSharedState.h"
#include "OpenALEnumTranslation.h"
#include "OpenALBuffer.h"
#include "OpenALSource.h"

static OpenALContext *globalOpenALContext = nullptr;

OpenALContext::OpenALContext(Logger *logger) {
    assert(logger);
    assert(globalOpenALContext == nullptr); // We only support one context at a time.
    globalOpenALContext = this;

    _state = std::make_unique<OpenALSharedState>(logger);

    // Note that we can list devices with alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER).
    ALCdevice *device = alcOpenDevice(nullptr); // Open default device.
    if (!device) {
        _state->logOpenALError("alcOpenDevice");
        return;
    }
    _state->setDevice(device);

    ALCcontext *context = alcCreateContext(device, nullptr);
    if (!context) {
        _state->logOpenALError("alcCreateContext");
        releaseDeviceAndContext();
        return;
    }
    _state->setContext(context);

    if (alcMakeContextCurrent(context) != ALC_TRUE) {
        _state->logOpenALError("alcMakeContextCurrent");
        releaseDeviceAndContext();
        return;
    }

    _initialized = true;

    // Use sane defaults.
    alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
    alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
    ALfloat orientation[] = {0.f, 1.f, 0.f, 0.f, 0.f, 1.f};
    alListenerfv(AL_ORIENTATION, orientation);
}

OpenALContext::~OpenALContext() {
    assert(globalOpenALContext == this);
    globalOpenALContext = nullptr;

    releaseDeviceAndContext();
    _state.reset(); // Will assert if there are any buffers or sources left.
}

void OpenALContext::setListenerPosition(Vec3f position) {
    _state->assertContext();

    alListener3f(AL_POSITION, position.x, position.y, position.z);
    _state->maybeLogOpenALError("alListener3f");
}

Vec3f OpenALContext::listenerPosition() {
    _state->assertContext();

    Vec3f result;
    alGetListener3f(AL_POSITION, &result.x, &result.y, &result.z);
    _state->maybeLogOpenALError("alGetListener3f");
    return result;
}

std::shared_ptr<AudioBuffer> OpenALContext::createBuffer(const PcmData &pcm) {
    _state->assertContext();

    if (!pcm.data)
        return _state->emptyBuffer();
    assert(pcm.frequency != 0);

    ALuint bufferId = 0;
    alGenBuffers(1, &bufferId);
    if (ALenum errorCode = alGetError(); errorCode != AL_NO_ERROR) {
        _state->logOpenALError("alGenBuffers");
        return nullptr;
    }

    std::shared_ptr<AudioBuffer> result = std::make_shared<OpenALBuffer>(_state.get(), bufferId);

    ALenum format = translatePcmFormat(pcm.bitness, pcm.channels);
    alBufferData(bufferId, format, pcm.data.data(), pcm.data.size(), pcm.frequency);
    if (ALenum errorCode = alGetError(); errorCode != AL_NO_ERROR) {
        _state->logOpenALError("alBufferData", errorCode);
        return nullptr;
    }

    return result;
}

std::unique_ptr<AudioSource> OpenALContext::createSource() {
    _state->assertContext();

    ALuint sourceId;
    alGenSources(1, &sourceId);
    if (ALenum errorCode = alGetError(); errorCode != AL_NO_ERROR) {
        _state->logOpenALError("alGenSources");
        return nullptr;
    }

    return std::make_unique<OpenALSource>(_state.get(), sourceId);
}

void OpenALContext::releaseDeviceAndContext() {
    if (_state->context()) {
        if (alcMakeContextCurrent(nullptr) == ALC_FALSE)
            _state->logOpenALError("alcMakeContextCurrent");

        _state->clearOpenALError();
        alcDestroyContext(_state->context());
        _state->maybeLogOpenALError("alcDestroyContext");

        _state->setContext(nullptr);
    }

    if (_state->device()) {
        if (alcCloseDevice(_state->device()) == ALC_FALSE) // Closing a device will fail if the device contains any contexts or buffers.
            _state->logOpenALError("alcCloseDevice");
        _state->setDevice(nullptr);
    }
}

std::unique_ptr<AudioContext> AudioContext::createStandardContext(Logger *logger) {
    return std::make_unique<OpenALContext>(logger);
}
