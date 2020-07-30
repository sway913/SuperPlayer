//
// Created by ldr on 2020/7/16.
//
#include "VideoEncoder.h"
#include "MediaCodecEncoder.h"

VideoEncoder *VideoEncoder::getEncoder(JNIEnv *env, bool is_support_media_codec) {
    if (is_support_media_codec) {
        return new MediaCodecEncoder(env);
    }
    return nullptr;
}

VideoEncoder::~VideoEncoder() = default;
