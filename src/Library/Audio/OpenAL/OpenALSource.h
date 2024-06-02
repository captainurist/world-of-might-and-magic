#pragma once

#include <queue>
#include <memory>
#include <vector>

#include <AL/al.h> // NOLINT: not a C system header.

#include "Library/Audio/Interface/AudioSource.h"

class OpenALSharedState;

class OpenALSource : public AudioSource {
 public:
    OpenALSource(OpenALSharedState *state, ALuint sourceId);
    virtual ~OpenALSource();

    virtual Vec3f position() const override;
    virtual void setPosition(Vec3f position) override;

    virtual bool isLooping() const override;
    virtual void setLooping(bool looping) override;

    virtual float volume() const override;
    virtual void setVolume(float volume) override;

    virtual AudioState state() const override;
    virtual void setState(AudioState state) override;

    virtual void enqueueBuffer(std::shared_ptr<AudioBuffer> buffer) override;
    virtual void clearBuffers() override;
    virtual int queuedBufferCount() override;

 private:
    OpenALSharedState *_state = nullptr;
    ALuint _sourceId = 0;
    std::queue<std::shared_ptr<AudioBuffer>> _buffers;
    std::vector<ALuint> _tmp;
};
