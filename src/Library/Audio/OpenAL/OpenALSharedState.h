#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <AL/al.h> // NOLINT: not a system C header.
#include <AL/alc.h> // NOLINT: not a system C header.

class Logger;
class LogCategory;
class AudioBuffer;
class OpenALBuffer;
class OpenALSource;

class OpenALSharedState {
 public:
    explicit OpenALSharedState(Logger *logger);
    ~OpenALSharedState();

    inline void assertContext() const {
        assert(_context == nullptr || alcGetCurrentContext() == _context);
    }

    void clearOpenALError();
    void logOpenALError(const char *functionName);
    void logOpenALError(const char *functionName, ALenum errorCode);
    void maybeLogOpenALError(const char *functionName);
    const LogCategory &logCategory() const;
    Logger *logger() const;

    void setDevice(ALCdevice *device);
    ALCdevice *device() const;

    void setContext(ALCcontext *context);
    ALCcontext *context() const;

    void registerBuffer(OpenALBuffer *buffer);
    void unregisterBuffer(OpenALBuffer *buffer);
    std::shared_ptr<AudioBuffer> emptyBuffer() const;

    void registerSource(OpenALSource *source);
    void unregisterSource(OpenALSource *source);

 private:
    Logger *_logger = nullptr;
    ALCdevice *_device = nullptr;
    ALCcontext *_context = nullptr;
    std::shared_ptr<AudioBuffer> _emptyBuffer;
    size_t _bufferCount = 0;
    size_t _sourceCount = 0;
};
