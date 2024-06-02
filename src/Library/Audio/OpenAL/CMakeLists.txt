cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(LIBRARY_AUDIO_OPENAL_SOURCES
        OpenALBuffer.cpp
        OpenALContext.cpp
        OpenALSharedState.cpp
        OpenALEnumTranslation.cpp
        OpenALSource.cpp)

set(LIBRARY_AUDIO_OPENAL_HEADERS
        OpenALBuffer.h
        OpenALContext.h
        OpenALSharedState.h
        OpenALEnumTranslation.h
        OpenALSource.h)

add_library(library_audio_openal STATIC ${LIBRARY_AUDIO_OPENAL_SOURCES} ${LIBRARY_AUDIO_OPENAL_HEADERS})
target_check_style(library_audio_openal)
target_link_libraries(library_audio_openal PUBLIC library_audio_interface PRIVATE OpenAL::OpenAL)

add_library(library_audio_implementation INTERFACE)
target_link_libraries(library_audio_implementation INTERFACE library_audio_openal)
