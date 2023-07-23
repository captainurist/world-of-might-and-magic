#pragma once

#include <cassert>
#include <utility>

#include "Texture.h"

class TextureHandle {
public:
    TextureHandle() = default;
    TextureHandle(const TextureHandle &) = delete; // Non-copyable.
    TextureHandle(TextureHandle &&other) {
        swap(*this, other);
    }

    TextureHandle &operator=(const TextureHandle &) = delete; // Non-copyable.

    TextureHandle &operator=(TextureHandle &&other) {
        swap(*this, other);
        return *this;
    }

    explicit TextureHandle(Texture *texture): _texture(texture) {
        if (_texture)
            ++_texture->_refCount;
    }

    ~TextureHandle() {
        if (_texture)
            --_texture->_refCount;
    }

    Texture *operator->() const {
        assert(_texture);
        return _texture;
    }

    void reset() {
        *this = TextureHandle();
    }

    friend void swap(TextureHandle &l, TextureHandle &r) {
        std::swap(l._texture, r._texture);
    }

    explicit operator bool() const {
        return _texture;
    }

 private:
    Texture *_texture = nullptr;
};


