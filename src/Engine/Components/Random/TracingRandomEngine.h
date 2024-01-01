#pragma once

#include <memory>

#include "Library/Random/RandomEngine.h"

class Platform;

class TracingRandomEngine : public RandomEngine {
 public:
    explicit TracingRandomEngine(Platform *platform, RandomEngine *base);

    virtual uint32_t random() override;
    virtual void seed(uint32_t seed) override;
    virtual uint32_t peek() const override;
    virtual int mapUniform(uint32_t entropy, int hi) const override;
    virtual float mapUniformFloat(uint32_t entropy) const override;

 private:
    void printTrace(const char *function, uint32_t value) const;

 private:
    Platform *_platform = nullptr;
    RandomEngine *_base = nullptr;
};
