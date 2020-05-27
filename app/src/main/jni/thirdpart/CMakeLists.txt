cmake_minimum_required(VERSION 3.4.1)

project(thirdpart)

add_library(thirdpart
        STATIC
)

target_link_libraries(
        thirdpart
        oboe
        soundtouch
)

ADD_SUBDIRECTORY(oboe-1.3)
ADD_SUBDIRECTORY(soundtouch)
