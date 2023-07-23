#pragma once

#include <unordered_map>

#include "Utility/Memory/Blob.h"
#include "Utility/MapAccess.h"

#include "ImageAnimation.h"

class AnimationTableEntry;

class AnimationTable {
public:
    void loadFromFile(const Blob &mm7data);

    /**
     * @param name                      Animation name.
     * @return                          Animation for the given name, or `nullptr` if such animation doesn't exist.
     */
    const ImageAnimation *animation(const std::string &name) {
        return valuePtr(_animationByName, name);
    }

private:
    void importAnimations(const std::vector<AnimationTableEntry> &entries);

private:
    std::unordered_map<std::string, ImageAnimation> _animationByName;
};
