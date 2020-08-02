//
// Created by ldr on 2020/7/16.
//
#include "VideoEncoder.h"
#include "MediaCodecEncoder.h"

VideoEncoder *VideoEncoder::getEncoder(JNIEnv *env, const char *path, int w, int h, bool is_support_media_codec) {
    if (is_support_media_codec) {
        return new MediaCodecEncoder(env, w, h, path);
    }
    return nullptr;
}

VideoEncoder::~VideoEncoder() = default;
