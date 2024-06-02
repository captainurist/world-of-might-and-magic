cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(LIBRARY_MEDIA_DECODER_SOURCES
        MediaPacket.cpp
        MediaStream.cpp)

set(LIBRARY_MEDIA_DECODER_HEADERS
        MediaEnums.h
        MediaInfo.h
        MediaPacket.h
        MediaStream.h)

add_library(library_media_decoder STATIC ${LIBRARY_MEDIA_DECODER_SOURCES} ${LIBRARY_MEDIA_DECODER_HEADERS})
target_link_libraries(library_media_decoder PUBLIC utility)
target_link_libraries(library_media_decoder PRIVATE ${AVFORMAT_LIBRARIES} ${AVCODEC_LIBRARIES} ${AVUTIL_LIBRARIES} ${SWSCALE_LIBRARIES} ${SWRESAMPLE_LIBRARIES})
target_include_directories(library_media_decoder PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_check_style(library_media_decoder)
