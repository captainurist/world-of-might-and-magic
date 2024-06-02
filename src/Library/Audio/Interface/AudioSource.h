#pragma once

#include <memory>

#include "Library/Geometry/Vec.h"

#include "AudioEnums.h"

class AudioBuffer;

class AudioSource {
 public:
    virtual ~AudioSource() = default;

    virtual Vec3f position() const = 0;
    virtual void setPosition(Vec3f position) = 0;

    virtual bool isLooping() const = 0;
    virtual void setLooping(bool looping) = 0;

    virtual float volume() const = 0; // [0, 1]
    virtual void setVolume(float volume) = 0;

    // Pitch is not exposed through this interface - why would you need to speed up the playback, anyway?

    virtual AudioState state() const = 0;
    virtual void setState(AudioState state) = 0;

    // Buffer must belong to the same context, otherwise the function will assert.
    virtual void enqueueBuffer(std::shared_ptr<AudioBuffer> buffer) = 0; // Also drops the buffers that have already played.
    virtual void clearBuffers() = 0; // Clears & stops.
    virtual int queuedBufferCount() = 0; // How many buffers do we have left, including the one that's being played now?

    // Sugar methods:

    void play() {
        setState(AUDIO_PLAYING);
    }

    void pause() {
        setState(AUDIO_PLAYING);
    }

    void stop() {
        setState(AUDIO_PLAYING);
    }
};
