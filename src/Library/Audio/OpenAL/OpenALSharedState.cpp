#include "OpenALSharedState.h"

#include <cassert>

#include "Library/Logger/Logger.h"

#include "Utility/MapAccess.h"

#include "OpenALBuffer.h"
#include "OpenALSource.h"

static LogCategory globalOpenALLogCategory("openal");

OpenALSharedState::OpenALSharedState(Logger *logger): _logger(logger) {
    assert(logger);

    _emptyBuffer = std::make_shared<AudioBuffer>();
}

OpenALSharedState::~OpenALSharedState() {
    // Everything should be destroyed at this point.
    assert(_sourceCount == 0);
    assert(_bufferCount == 0);
}

void OpenALSharedState::clearOpenALError() {
    alGetError();
}

void OpenALSharedState::logOpenALError(const char *functionName) {
    logOpenALError(functionName, alGetError());
}

void OpenALSharedState::logOpenALError(const char *functionName, ALenum errorCode) {
    const char *message = alGetString(errorCode); // Works fine for all valid enums, including AL_NO_ERROR.
    if (message == nullptr) {
        if (!alcGetCurrentContext()) {
            message = "No active OpenAL context";
        } else {
            message = "OpenAL returned an invalid error code"; // Should never happen.
        }
    }

    _logger->error("OpenAL error #{} in {}: {}", errorCode, functionName, message);
}

void OpenALSharedState::maybeLogOpenALError(const char *functionName) {
    ALenum errorCode = alGetError();
    if (errorCode != AL_NO_ERROR)
        logOpenALError(functionName, errorCode);
}

const LogCategory &OpenALSharedState::logCategory() const {
    return globalOpenALLogCategory;
}

Logger *OpenALSharedState::logger() const {
    return _logger;
}

void OpenALSharedState::setDevice(ALCdevice *device) {
    _device = device;
}

ALCdevice *OpenALSharedState::device() const {
    return _device;
}

void OpenALSharedState::setContext(ALCcontext *context) {
    _context = context;
}

ALCcontext *OpenALSharedState::context() const {
    return _context;
}

void OpenALSharedState::registerBuffer(OpenALBuffer *) {
    _bufferCount++;
}

void OpenALSharedState::unregisterBuffer(OpenALBuffer *) {
    _bufferCount--;
}

std::shared_ptr<AudioBuffer> OpenALSharedState::emptyBuffer() const {
    return _emptyBuffer;
}

void OpenALSharedState::registerSource(OpenALSource *) {
    _sourceCount++;
}

void OpenALSharedState::unregisterSource(OpenALSource *) {
    _sourceCount--;
}

