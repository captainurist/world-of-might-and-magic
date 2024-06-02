cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(LIBRARY_PCM_SOURCES
        PcmData.cpp)

set(LIBRARY_PCM_HEADERS
        PcmData.h
        PcmEnums.h)

add_library(library_pcm STATIC ${LIBRARY_PCM_SOURCES} ${LIBRARY_PCM_HEADERS})
target_link_libraries(library_pcm PUBLIC utility)
target_check_style(library_pcm)
