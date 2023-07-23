cmake_minimum_required(VERSION 3.20.4 FATAL_ERROR)

set(ENGINE_IMAGES_SOURCES
        AnimationTable.cpp
        ImageLoader.cpp
        TextureCache.cpp)

set(ENGINE_IMAGES_HEADERS
        AnimationTable.h
        AnimationTableEntry.h
        ImageAnimation.h
        ImageId.h
        ImageLoader.h
        Texture.h
        TextureHandle.h
        TextureCache.h)

add_library(engine_images STATIC ${ENGINE_IMAGES_SOURCES} ${ENGINE_IMAGES_HEADERS})
target_link_libraries(engine_images PUBLIC utility library_image)

target_check_style(engine_images)
