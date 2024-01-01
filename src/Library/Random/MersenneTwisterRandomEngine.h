#pragma once

#include <cassert>
#include <random>

#include "RandomEngine.h"

class MersenneTwisterRandomEngine: public RandomEngine {
 public:
    virtual uint32_t random() override {
        return _base();
    }

    virtual void seed(uint32_t seed) override {
        if (seed == 0) {
            _base = std::mt19937();
        } else {
            _base.seed(seed);
        }
    }

    virtual uint32_t peek() const override {
        std::mt19937 copy = _base;
        return copy();
    }

    virtual int mapUniform(uint32_t entropy, int hi) const override {
        assert(hi > 0);
        return static_cast<int>((static_cast<uint64_t>(entropy) * static_cast<uint64_t>(hi)) >> 32);
    }

    virtual float mapUniformFloat(uint32_t entropy) const override {
        return entropy / static_cast<float>(1ull << 32);
    }

 private:
    std::mt19937 _base;
};
