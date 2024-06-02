#pragma once

#include <memory>

#include "Library/Geometry/Vec.h"

#include "Library/Pcm/PcmData.h"

class AudioBuffer;
class AudioSource;
class Logger;

class AudioContext {
 public:
    virtual ~AudioContext() = default;

    static std::unique_ptr<AudioContext> createStandardContext(Logger *logger);

    virtual void setListenerPosition(Vec3f position) = 0;
    virtual Vec3f listenerPosition() = 0;

    // Note on allocations.
    //
    // Sources should be pooled and reused by the user. So allocations matter little.
    //
    // Buffer allocations are negligible really, but if it is a concern then it's possible to:
    // - create a memory arena in the derived `AudioContext`.
    // - use std::allocate_shared to allocate both the buffer instance and the shared_ptr control block in the arena.
    // Result: zero allocations per buffer instance.

    virtual std::shared_ptr<AudioBuffer> createBuffer(const PcmData &pcm) = 0;
    virtual std::unique_ptr<AudioSource> createSource() = 0;
};
