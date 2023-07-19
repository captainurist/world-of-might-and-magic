cmake_minimum_required(VERSION 3.20.4 FATAL_ERROR)

set(BUILD_TESTING OFF CACHE BOOL "Don't build tests for subprojects. Value is forced." FORCE)

# Allow overriding options in subprojects by setting a normal variable of the same name.
# Note that setting a CMAKE_POLICY_* variable is the intended way to pass this policy to a subproject.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

add_subdirectory(fast_float)
add_subdirectory(fmt)
add_subdirectory(glm)
add_subdirectory(magic_enum)
add_subdirectory(nuklear)
add_subdirectory(cli11)
add_subdirectory(nlohmann_json)
add_subdirectory(mio)

# glad
add_subdirectory(glad)
target_include_directories(glad INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

# luajit
add_subdirectory(luajit)
target_include_directories(libluajit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/luajit/luajit/src)
set_target_properties(libluajit PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS}" LINK_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}") # Make it compile under x86.

# mini
add_library(mini INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mini/src/mini/ini.h)
add_library(mini::mini ALIAS mini)
target_include_directories(mini INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mini/src)

# openal_soft
set(LIBTYPE STATIC)
set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_TESTS OFF)
add_subdirectory(openal_soft)

# SDL
if(BUILD_PLATFORM STREQUAL "linux")
    find_package(SDL2 CONFIG REQUIRED GLOBAL) # GLOBAL is required for SDL2 to be available in src/
    add_library(SDL2::SDL2OE ALIAS SDL2::SDL2) # Link dynamically on linux.
else()
    set(SDL_TEST OFF)
    set(SDL_SHARED OFF)
    set(SDL_STATIC ON) # We're only interested in static SDL.
    add_subdirectory(sdl)
    add_library(SDL2OE INTERFACE)
    target_link_libraries(SDL2OE INTERFACE SDL2::SDL2-static SDL2::SDL2main) # Link statically everywhere else.
    add_library(SDL2::SDL2OE ALIAS SDL2OE)
endif()

# ffmpeg
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan/conan.cmake"
            TLS_VERIFY ON)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/conan)

include(conan)

if(BUILD_PLATFORM STREQUAL "linux" OR BUILD_PLATFORM STREQUAL "android")
    set(FFMPEG_ADDITIONAL_OPTIONS
            ffmpeg:with_libalsa=False
            ffmpeg:with_pulse=False
            ffmpeg:with_vaapi=False
            ffmpeg:with_vdpau=False
            ffmpeg:with_vulkan=False
            ffmpeg:with_xcb=False)
elseif(BUILD_PLATFORM STREQUAL "darwin")
    set(FFMPEG_ADDITIONAL_OPTIONS
            ffmpeg:with_appkit=False
            ffmpeg:with_avfoundation=False
            ffmpeg:with_coreimage=False
            ffmpeg:with_audiotoolbox=False
            ffmpeg:with_videotoolbox=False)
else()
    set(FFMPEG_ADDITIONAL_OPTIONS)
endif()

# Options listed here:
# https://github.com/conan-io/conan-center-index/blob/master/recipes/ffmpeg/all/conanfile.py
conan_cmake_configure(
        REQUIRES ffmpeg/5.1
        GENERATORS cmake_find_package
        OPTIONS
        ffmpeg:shared=False
        ffmpeg:postproc=False
        ffmpeg:avfilter=False
        ffmpeg:with_zlib=False
        ffmpeg:with_bzip2=False
        ffmpeg:with_lzma=False
        ffmpeg:with_libiconv=False
        ffmpeg:with_freetype=False
        ffmpeg:with_openjpeg=False
        ffmpeg:with_openh264=False
        ffmpeg:with_opus=False
        ffmpeg:with_vorbis=False
        ffmpeg:with_libx264=False
        ffmpeg:with_libx265=False
        ffmpeg:with_libvpx=False
        ffmpeg:with_libmp3lame=False
        ffmpeg:with_libfdk_aac=False
        ffmpeg:with_libwebp=False
        ffmpeg:with_ssl=False
        ffmpeg:with_programs=False
        ${FFMPEG_ADDITIONAL_OPTIONS}
        ffmpeg:disable_all_encoders=True
        ffmpeg:disable_all_muxers=True
        ffmpeg:disable_all_bitstream_filters=True
        ffmpeg:disable_all_protocols=True
        ffmpeg:disable_all_devices=True
        ffmpeg:disable_all_filters=True)

conan_cmake_autodetect(CONAN_SETTINGS)

message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
message(STATUS "CONAN_SETTINGS=${CONAN_SETTINGS}")

function(dump_conan_settings SETTINGS FILE TAIL)
    string(REGEX REPLACE "arch=[0-9a-z_]*" "" SETTINGS "${SETTINGS}")
    string(REPLACE ";" "\n" SETTINGS "${SETTINGS}")
    message(STATUS ${SETTINGS})
    file(WRITE ${FILE} "[settings]\n${SETTINGS}\n${TAIL}")
endfunction()

if(BUILD_PLATFORM STREQUAL "linux" AND "-m32" IN_LIST CMAKE_C_FLAGS)
    set(CONAN_SETTINGS "${CONAN_SETTINGS};arch=x86")
elseif(BUILD_PLATFORM STREQUAL "android" AND CONAN_SETTINGS MATCHES "compiler.version=17;")
    string(REPLACE "compiler.version=17;" "compiler.version=16;" CONAN_SETTINGS "${CONAN_SETTINGS}")
endif()

if(BUILD_PLATFORM STREQUAL "windows" AND CONAN_SETTINGS MATCHES ".*arch=x86;.*")
    # Working around this issue:
    # https://github.com/conan-io/conan-center-index/issues/6190
    dump_conan_settings("${CONAN_SETTINGS}" "${CMAKE_BINARY_DIR}/conan/profile_x86" "arch=x86\nos=Windows")
    dump_conan_settings("${CONAN_SETTINGS}" "${CMAKE_BINARY_DIR}/conan/profile_x86_64" "arch=x86_64\nos=Windows")
    set(CONAN_INSTALL_ARGS PROFILE_HOST ${CMAKE_BINARY_DIR}/conan/profile_x86 PROFILE_BUILD ${CMAKE_BINARY_DIR}/conan/profile_x86_64)
else()
    set(CONAN_INSTALL_ARGS SETTINGS ${CONAN_SETTINGS})
endif()

message(STATUS "CONAN_INSTALL_ARGS=${CONAN_INSTALL_ARGS}")

conan_cmake_install(PATH_OR_REFERENCE .
        INSTALL_FOLDER ${CMAKE_BINARY_DIR}/conan
        OUTPUT_FOLDER ${CMAKE_BINARY_DIR}/conan
        BUILD missing
        REMOTE conancenter
        ${CONAN_INSTALL_ARGS})

find_package(ffmpeg REQUIRED GLOBAL)

if(BUILD_PLATFORM STREQUAL "windows")
    # TODO(captainurist): actually we need -disable-network option support in conan.
    message(STATUS "LINKING WITH ws2_32.lib")
    target_link_libraries(ffmpeg::avformat INTERFACE ws2_32.lib)
endif()

# googletest
if(ENABLE_TESTS)
    set(INSTALL_GTEST OFF)
    set(BUILD_GMOCK OFF)
    add_subdirectory(googletest)
endif()
