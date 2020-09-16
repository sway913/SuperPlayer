//
// Created by ldr on 2020/7/16.
//
#include "VideoEncoder.h"
#include "MediaCodecEncoder.h"
#include "FFVideoEncoder.h"

VideoEncoder *VideoEncoder::getEncoder(JNIEnv *env, const char *path, int w, int h, bool is_support_media_codec) {
    if (is_support_media_codec) {
        return new MediaCodecEncoder(env, w, h, path);
    }
    return new FFVideoEncoder(w, h, path);
}

VideoEncoder::~VideoEncoder() = default;
