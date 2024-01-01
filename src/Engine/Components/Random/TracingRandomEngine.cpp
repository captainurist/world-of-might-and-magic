#include "TracingRandomEngine.h"

#include <cassert>

#include "Library/Platform/Interface/Platform.h"
#include "Library/StackTrace/StackTrace.h"

#include "Utility/Format.h"

TracingRandomEngine::TracingRandomEngine(Platform *platform, RandomEngine *base): _platform(platform), _base(base) {
    assert(platform);
    assert(base);
}

uint32_t TracingRandomEngine::random() {
    uint32_t result = _base->random();
    printTrace("random", result);
    return result;
}

void TracingRandomEngine::seed(uint32_t seed) {
    _base->seed(seed);
}

uint32_t TracingRandomEngine::peek() const {
    return _base->peek();
}

int TracingRandomEngine::mapUniform(uint32_t entropy, int hi) const {
    return _base->mapUniform(entropy, hi);
}

float TracingRandomEngine::mapUniformFloat(uint32_t entropy) const {
    return _base->mapUniformFloat(entropy);
}

void TracingRandomEngine::printTrace(const char *function, uint32_t value) const {
    fmt::println(stderr, "TracingRandomEngine::{} called at {}ms, returning {}, stacktrace:",
                 function, _platform->tickCount(), value);
    printStackTrace(stderr);
}
