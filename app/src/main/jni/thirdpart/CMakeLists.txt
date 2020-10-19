cmake_minimum_required(VERSION 3.4.1)

project(thirdpart)

add_library(thirdpart
        STATIC
        empty.h
)

target_link_libraries(
        thirdpart
        oboe
        soundtouch
        sox
        png
        yuv
)

ADD_SUBDIRECTORY(oboe-1.3)
ADD_SUBDIRECTORY(soundtouch)
ADD_SUBDIRECTORY(sox)
ADD_SUBDIRECTORY(libpng)
ADD_SUBDIRECTORY(libyuv)
