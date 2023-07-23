#pragma once

//#include <optional>

//#include "Library/Image/Image.h"

//#include "ImageId.h"

#include "Engine/Graphics/TextureRenderId.h" // TODO(captainurist): move to Engine/Images or Engine/Textures?

class Texture {
public:
    //Texture2(const ImageId &id) : _id(id) {}

    //const ImageId &id() const {
    //    return _id;
    //}

    //const RgbaImage &image() const {
    //    return _image;
    //}

    [[nodiscard]] size_t width() const {
        return _width;
    }

    [[nodiscard]] size_t height() const {
        return _height;
    }

    [[nodiscard]] TextureRenderId hardwareId() const {
        return _renderId;
    }

    [[nodiscard]] bool isUnused() const {
        return _refCount == 0;
    }

private:
    friend class TextureHandle;
    friend class TextureCache;

private:
    //ImageId _id;
    //RgbaImage _image; // TODO(captainurist): doesn't belong here, drop!
    size_t _width = 0;
    size_t _height = 0;
    TextureRenderId _renderId;
    int _refCount = 0; // Number of `TextureHandle` object referencing this texture.
};

