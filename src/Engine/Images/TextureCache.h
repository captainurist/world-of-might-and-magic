#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "Library/Image/Image.h"

#include "ImageId.h"
#include "TextureHandle.h"
#include "Texture.h"

class ImageLoader2;
class IRender;

class TextureCache {
public:
    explicit TextureCache(ImageLoader2 *loader, IRender *renderer);

    // Loads & uploads to GPU, takes from cache if already loaded
    TextureHandle load(const ImageId &id);

    // Drops whatever's unused.
    void optimize();

private:
    TextureHandle store(const ImageId &id, std::unique_ptr<Texture> texture);

private:
    ImageLoader2 *_loader = nullptr;
    IRender *_renderer = nullptr;
    std::unordered_map<ImageId, std::unique_ptr<Texture>> _textureById;
    std::vector<std::unique_ptr<Texture>> _dynamicTextures;
};


// ResourceTextureCache: load only.
// DynamicTextureCache: store / update. Better a single function?
// Why cache at all? Why not a separate DynamicTexture?
//
// DynamicTexture t = DynamicTexture::create(IRender)
//                    DynamicTexture::create(IRender, RGBA)
// t.handle() => creates a handle that doesn't do shit but can be stored / passed around.
// t.update(RGBA)
// t.reset() => frees resources
//
// Inside:
// IRender *
// std::unique_ptr<Texture2>
// ~DynamicTexture --- frees resources
//

