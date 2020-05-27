set(libs_dir ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jni/libs)

add_library(fdk-aac STATIC IMPORTED)
set_target_properties(fdk-aac PROPERTIES IMPORTED_LOCATION ${libs_dir}/fdk-aac/lib/android/${ANDROID_ABI}/libfdk-aac.a)

add_library(avformat STATIC IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${libs_dir}/ffmpeg/lib/android/${ANDROID_ABI}/libavformat.a)

add_library(avcodec STATIC IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${libs_dir}/ffmpeg/lib/android/${ANDROID_ABI}/libavcodec.a)

add_library(avfilter STATIC IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${libs_dir}/ffmpeg/lib/android/${ANDROID_ABI}/libavfilter.a)

add_library(avutil STATIC IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${libs_dir}/ffmpeg/lib/android/${ANDROID_ABI}/libavutil.a)

add_library(postproc STATIC IMPORTED)
set_target_properties(postproc PROPERTIES IMPORTED_LOCATION ${libs_dir}/ffmpeg/lib/android/${ANDROID_ABI}/libpostproc.a)

add_library(swscale STATIC IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${libs_dir}/ffmpeg/lib/android/${ANDROID_ABI}/libswscale.a)

add_library(swresample STATIC IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${libs_dir}/ffmpeg/lib/android/${ANDROID_ABI}/libswresample.a)

add_library(mp3lame STATIC IMPORTED)
set_target_properties(mp3lame PROPERTIES IMPORTED_LOCATION ${libs_dir}/lame/lib/android/${ANDROID_ABI}/libmp3lame.a)

add_library(x264 STATIC IMPORTED)
set_target_properties(x264 PROPERTIES IMPORTED_LOCATION ${libs_dir}/x264/lib/android/${ANDROID_ABI}/libx264.a)

find_library(z-lib z)

set(ffmpeg_libs avformat avcodec avfilter avutil postproc swscale swresample mp3lame x264 fdk-aac ${z-lib})

include_directories(${libs_dir}/ffmpeg/include
        ${libs_dir}/lame/include
        ${libs_dir}/fdk-aac/include
        ${libs_dir}/x264/include)
