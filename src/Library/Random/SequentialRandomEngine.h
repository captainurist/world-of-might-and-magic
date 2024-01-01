#pragma once

#include <cassert>

#include "RandomEngine.h"

class SequentialRandomEngine : public RandomEngine {
 public:
    virtual uint32_t random() override {
        return ++_state;
    }

    virtual void seed(uint32_t seed) override {
        _state = seed;
    }

    virtual uint32_t peek() const override {
        return _state + 1;
    }

    virtual int mapUniform(uint32_t entropy, int hi) const override {
        assert(hi > 0);
        return entropy % hi;
    }

    virtual float mapUniformFloat(uint32_t entropy) const override {
        return (entropy % 256) / 256.0f;
    }

 private:
    uint32_t _state = 0; // Using unsigned here so that it wraps around safely.
};
