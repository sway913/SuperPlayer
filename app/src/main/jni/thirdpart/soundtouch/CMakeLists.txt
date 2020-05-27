cmake_minimum_required(VERSION 3.4.1)

project(soundtouch)

include_directories(soundtouch/include)

file(GLOB_RECURSE source src/*)

add_library(soundtouch
        STATIC
        ${source})
