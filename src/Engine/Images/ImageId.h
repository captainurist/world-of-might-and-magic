#pragma once

#include <string>
#include <utility>

// TODO(captainurist): drop once we have filesystem abstractions.
enum class ImageSource {
    IMAGE_SOURCE_ICONS_LOD,     // Stored in icons.lod.
    IMAGE_SOURCE_SPRITES_LOD,   // Stored in sprites.lod.
    IMAGE_SOURCE_BITMAPS_LOD,   // Stored in bitmaps.lod.
    IMAGE_SOURCE_FILE,          // Stored as a file on the file system.
};
using enum ImageSource;

enum class ImageContainerFormat {
    IMAGE_CONTAINER_PCX,            // Just a pcx file.
    IMAGE_CONTAINER_COMPRESSED_PCX, // Compressed pcx file in a LOD, compression header is `TextureHeader`.
    IMAGE_CONTAINER_MM_TEXTURE,     // Compressed or uncompressed texture in a custom MM format in a LOD, header is `TextureHeader`.
    IMAGE_CONTAINER_MM_SPRITE,      // Compressed or uncompressed texture in a custom MM sprite format in a LOD, header is `LODSpriteHeader`.
};
using enum ImageContainerFormat;

/**
 * This struct identifies a single image in OpenEnroth resources, with the info on how to read & prepare it.
 */
struct ImageId {
    ImageSource source = IMAGE_SOURCE_FILE;
    ImageContainerFormat containerFormat = IMAGE_CONTAINER_PCX;
    std::string path; // Assumed to be result of a call to `MakeDataPath`.
    Color mask;

    friend bool operator==(const ImageId &l, const ImageId &r) = default;

    static ImageId pcx(std::string path) {
        ImageId result;
        result.source = IMAGE_SOURCE_FILE;
        result.containerFormat = IMAGE_CONTAINER_PCX;
        result.path = std::move(path);
        return result;
    }

    static ImageId lodPcx(ImageSource source, std::string path, Color mask = Color()) {
        assert(source != IMAGE_SOURCE_FILE);
        ImageId result;
        result.source = source;
        result.containerFormat = IMAGE_CONTAINER_COMPRESSED_PCX;
        result.path = std::move(path);
        result.mask = mask;
        return result;
    }
};

template<>
struct std::hash<ImageId> {
    size_t operator()(const ImageId &value) const {
        return
            std::hash<std::string>()(value.path) ^
            std::hash<ImageSource>()(value.source) * 1610612741 ^
            std::hash<ImageContainerFormat>()(value.containerFormat) * 805306457;
    }
};
