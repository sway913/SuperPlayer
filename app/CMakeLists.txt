cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include(src/main/jni/libs/libs.cmake)

add_compile_options(
        -Werror=return-type
        -Wno-macro-redefined
)

file(GLOB_RECURSE common_src src/main/jni/common/*)
file(GLOB_RECURSE audio_src src/main/jni/audio/*)
file(GLOB_RECURSE video_src src/main/jni/video/*)


add_library(
        superplayer
        SHARED
        src/main/jni/super_player_jni.cpp
        ${common_src}
        ${audio_src}
        ${video_src}
)


find_library(
        log-lib
        log
)

target_link_libraries(
        superplayer
        thirdpart
        ${libs}
        ${log-lib}
        GLESv2
        EGL
        -landroid
)

ADD_SUBDIRECTORY(src/main/jni/thirdpart)

