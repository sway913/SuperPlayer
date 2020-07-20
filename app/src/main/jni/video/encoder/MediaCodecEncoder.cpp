//
// Created by ldr on 2020/7/16.
//

#include "MediaCodecEncoder.h"

MediaCodecEncoder::MediaCodecEncoder(JNIEnv *env) {
    if (env) {
//        env->GetJavaVM(&javaVm);
//        jclass cls = env->FindClass("com/smzh/superplayer/video/VideoEncoder");
//        jmethodID init_id = env->GetMethodID(cls, "<init>", "(IIIILjava/lang/String;)V");
//        jobject encoder = env->NewObject(cls, init_id,0,0,0,0);
//        video_encoder = env->NewGlobalRef(encoder);
//        encode_id = env->GetMethodID(cls,"encodeFrame","()V");
//        stop_id = env->GetMethodID(cls,"stopEncoder","()V");
    }
}


void MediaCodecEncoder::start() {

}

void MediaCodecEncoder::resume() {

}

void MediaCodecEncoder::pause() {

}

void MediaCodecEncoder::stop() {

}

void MediaCodecEncoder::encodeFrame() {

}