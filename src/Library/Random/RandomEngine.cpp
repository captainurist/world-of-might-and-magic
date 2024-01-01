#include "RandomEngine.h"

#include <cassert>

int RandomEngine::random(int hi) {
    return mapUniform(random(), hi);
}

int RandomEngine::peek(int hi) const {
    return mapUniform(peek(), hi);
}

float RandomEngine::randomFloat() {
    return mapUniformFloat(random());
}

int RandomEngine::randomInSegment(int min, int max) {
    assert(max >= min);

    return min + random(max - min + 1);
}

int RandomEngine::randomDice(int count, int faces) {
    assert(count >= 0 && faces >= 0);

    if (count == 0 || faces == 0)
        return 0;

    int result = 0;
    for (int i = 0; i < count; i++)
        result += 1 + random(faces);
    return result;
}
