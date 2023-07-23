#include "ImageLoader.h"

#include "Engine/LOD.h"
#include "Engine/Serialization/CommonImages.h"

#include "Library/Binary/ContainerSerialization.h"
#include "Library/Compression/Compression.h"
#include "Library/Image/PCX.h"
#include "Library/Image/ImageFunctions.h"

#include "Utility/Streams/BlobInputStream.h"
#include "Utility/Exception.h"

ImageLoader2::ImageLoader2(LOD::File *iconsLod, LOD::File *bitmapsLod, LOD::File *spritesLod) : _iconsLod(iconsLod), _bitmapsLod(bitmapsLod), _spritesLod(spritesLod) {
    assert(iconsLod);
    assert(bitmapsLod);
    assert(spritesLod);
}

RgbaImage ImageLoader2::load(const ImageId &id) {
    Blob blob = blobFor(id);

    Palette palette;

    switch (id.containerFormat) {
    case IMAGE_CONTAINER_PCX:
        return applyMask(PCX::Decode(blob), id.mask);
    case IMAGE_CONTAINER_COMPRESSED_PCX:
        return applyMask(PCX::Decode(deserializeFakeTexture(blob)), id.mask);
    case IMAGE_CONTAINER_MM_TEXTURE:
        return applyMask(deserializeRealTexture(blob), id.mask);
    case IMAGE_CONTAINER_MM_SPRITE:
        return deserializeSprite(blob);
    }
}

LOD::File *ImageLoader2::lodFor(ImageSource source) {
    switch (source) {
    case IMAGE_SOURCE_ICONS_LOD: return _iconsLod;
    case IMAGE_SOURCE_SPRITES_LOD: return _spritesLod;
    case IMAGE_SOURCE_BITMAPS_LOD: return _bitmapsLod;
    default: return nullptr;
    }
}

Blob ImageLoader2::blobFor(const ImageId &id) {
    if (id.source == IMAGE_SOURCE_FILE) {
        return Blob::fromFile(id.path);
    } else {
        return lodFor(id.source)->LoadRaw(id.path);
    }
}

Blob ImageLoader2::deserializeFakeTexture(const Blob &data) {
    BlobInputStream stream(data);
    TextureHeader header;
    deserialize(stream, &header);

    Blob result = stream.readBlobOrFail(header.uTextureSize);
    if (header.uDecompressedSize)
        result = zlib::Uncompress(result, header.uDecompressedSize);
    return result;
}

RgbaImage ImageLoader2::deserializeRealTexture(const Blob &data) {
    BlobInputStream stream(data);
    TextureHeader header;
    deserialize(stream, &header);

    Blob pixels = stream.readBlobOrFail(header.uTextureSize);
    if (header.uDecompressedSize)
        pixels = zlib::Uncompress(pixels, header.uDecompressedSize);

    if (pixels.size() != header.uTextureWidth * header.uTextureHeight) {
        std::string name;
        deserialize(header.pName, &name); // Safe array<char> -> std::string conversion that can't overread.
        throw Exception("LOD image '{}' is corrupted: expected {}x{}={} pixels, got {} pixels",
                        name, header.uTextureWidth, header.uTextureHeight, header.uTextureWidth * header.uTextureHeight, pixels.size());
    }

    Palette palette = deserializePalette(stream.readBlobOrFail(0x300));
    return makeRgbaImage(GrayscaleImageView(static_cast<const uint8_t *>(pixels.data()), header.uTextureWidth, header.uTextureHeight), palette);

    // TODO(captainurist): Is this needed?
    // if (header->pBits & 2)
    //    pOutTex->pLevelOfDetail1 = &pOutTex->paletted_pixels[header->uSizeOfMaxLevelOfDetail];
}

Palette ImageLoader2::deserializePalette(const Blob &data) {
    assert(data.size() == 0x300);

    Palette result;

    const uint8_t *pos = static_cast<const uint8_t *>(data.data());
    for (size_t index = 0; index < 0x100; index++, pos += 3)
        result.colors[index] = Color(pos[0], pos[1], pos[2]);

    return result;
}

RgbaImage ImageLoader2::deserializeSprite(const Blob &data) {
    BlobInputStream stream(data);
    LODSpriteHeader header;
    deserialize(stream, &header);

    std::vector<LODSpriteLine> lines;
    deserialize(stream, &lines, tags::presized(header.uHeight));

    // TODO(captainurist): validate header & line data.

    Blob pixels = stream.readBlob(header.uSpriteSize);
    if (header.uDecompressedSize)
        pixels = zlib::Uncompress(pixels, header.uDecompressedSize);

    Image result = RgbaImage::solid(header.uWidth, header.uHeight, Color());

    for (size_t y = 0; y < header.uHeight; y++) {
        if (lines[y].begin < 0)
            continue;

        const uint8_t *src = static_cast<const uint8_t *>(pixels.data()) + lines[y].offset;
        Color *dst = result[y].data() + lines[y].begin;
        for (size_t i = 0, size = lines[y].end - lines[y].begin; i < size; i++, src++, dst++)
            *dst = Color(*src, 0, 0);
    }

    return result;
}

RgbaImage ImageLoader2::applyMask(RgbaImage image, Color mask) {
    if (mask == Color())
        return std::move(image);

    for (Color &pixel : image.pixels())
        if (pixel == mask)
            pixel = Color();

    return std::move(image);
}
