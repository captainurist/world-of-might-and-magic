cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(LIBRARY_AUDIO_INTERFACE_SOURCES)

set(LIBRARY_AUDIO_INTERFACE_HEADERS
        AudioBuffer.h
        AudioContext.h
        AudioEnums.h
        AudioSource.h
        ../../Pcm/PcmData.h)

add_library(library_audio_interface INTERFACE ${LIBRARY_AUDIO_INTERFACE_SOURCES} ${LIBRARY_AUDIO_INTERFACE_HEADERS})
target_check_style(library_audio_interface)
target_link_libraries(library_audio_interface INTERFACE library_geometry)
