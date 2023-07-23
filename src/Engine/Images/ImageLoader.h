#pragma once

#include "Library/Image/Image.h"
#include "Library/Image/Palette.h"

#include "ImageId.h"

namespace LOD {
class File;
} // namespace LOD

// TODO(captainurist): rename
class ImageLoader2 {
public:
    ImageLoader2(LOD::File *iconsLod, LOD::File *bitmapsLod, LOD::File *spritesLod);

    RgbaImage load(const ImageId &id);

private:
    LOD::File *lodFor(ImageSource source);
    Blob blobFor(const ImageId &id);
    static Blob deserializeFakeTexture(const Blob &data);
    static RgbaImage deserializeRealTexture(const Blob &data);
    static Palette deserializePalette(const Blob &data);
    static RgbaImage deserializeSprite(const Blob &data);
    static RgbaImage applyMask(RgbaImage image, Color mask);

private:
    LOD::File *_iconsLod = nullptr;
    LOD::File *_bitmapsLod = nullptr;
    LOD::File *_spritesLod = nullptr;
};
