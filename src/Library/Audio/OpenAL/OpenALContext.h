#pragma once

#include <memory>

#include "Library/Audio/Interface/AudioContext.h"

class OpenALSharedState;

/**
 * @see https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf
 */
class OpenALContext : public AudioContext {
 public:
    explicit OpenALContext(Logger *logger);
    virtual ~OpenALContext();

    virtual void setListenerPosition(Vec3f position) override;
    virtual Vec3f listenerPosition() override;

    virtual std::shared_ptr<AudioBuffer> createBuffer(const PcmData &pcm) override;
    virtual std::unique_ptr<AudioSource> createSource() override;

 private:
    void releaseDeviceAndContext();

 private:
    bool _initialized = false;
    std::unique_ptr<OpenALSharedState> _state;
};
