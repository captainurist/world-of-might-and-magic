#include "TextureCache.h"

#include <cassert>

#include "Engine/Graphics/IRender.h"

#include "ImageLoader.h"

TextureCache::TextureCache(ImageLoader2 *loader, IRender *renderer): _loader(loader), _renderer(renderer) {
    assert(loader);
    assert(renderer);
}

TextureHandle TextureCache::load(const ImageId &id) {
    auto pos = _textureById.find(id);
    if (pos != _textureById.end())
        return TextureHandle(pos->second.get());

    RgbaImage image = _loader->load(id);

    std::unique_ptr<Texture> texture = std::make_unique<Texture>();
    texture->_width = image.width();
    texture->_height = image.height();
    texture->_hardwareId = _renderer->CreateTexture(image);

    return store(id, std::move(texture));
}

void TextureCache::optimize() {
    for (const auto &[_, texture] : _textureById)
        if (texture->isUnused())
            _renderer->ReleaseTexture(texture->hardwareId());

    std::erase_if(_textureById, [](const auto &pair) {
        return pair.second->isUnused();
    });
}

TextureHandle TextureCache::store(const ImageId &id, std::unique_ptr<Texture> texture) {
    return TextureHandle(_textureById.emplace(id, std::move(texture)).first->second.get());
}
