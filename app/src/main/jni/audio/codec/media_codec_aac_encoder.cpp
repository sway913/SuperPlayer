//
// Created by Lvlingling on 2020/9/1.
//

#include "media_codec_aac_encoder.h"
#include "../../common/android_log.h"

MediaCodecAACEncoder::MediaCodecAACEncoder(JNIEnv *e) {
    e->GetJavaVM(&javaVm);
    this->env = e;
    jclass clazz = env->FindClass("com/smzh/superplayer/video/AudioEncoder");
    if (clazz == nullptr) {
        LOGI("video source not find video decoder");
    }
    jmethodID general_id = env->GetMethodID(clazz, "<init>", "()V");
    if (general_id == nullptr) {
        LOGI("video source not find video decoder init");
    }
    jobject decoder = env->NewObject(clazz, general_id);
    j_encoder = env->NewGlobalRef(decoder);

    init_id = env->GetMethodID(clazz, "init", "(Ljava/lang/String;II)V");
    close_id = env->GetMethodID(clazz, "endEncode", "()V");
    encode_id = env->GetMethodID(clazz, "encodeFrame", "([BJ)V");
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(decoder);
}

void MediaCodecAACEncoder::init(int in_sample, int in_channels, const char *file_name) {
    this->sample = in_sample;
    this->channels = in_channels;
    if (j_encoder == nullptr) {
        LOGE("Decode Object is null when open");
        return;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }

    jstring p = env->NewStringUTF(file_name);
    env->CallVoidMethod(j_encoder, init_id, p, in_sample, in_channels);
    env->DeleteLocalRef(p);
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

void MediaCodecAACEncoder::encode(short *data, int len) {
    if (j_encoder == nullptr) {
        LOGE("Decode Object is null when open");
        return;
    }
    len = len * sizeof(short);
    if (len <= 0) {
       len = 0;
    }

    encode_byte += len;

    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    jbyteArray array = env->NewByteArray(len);
    env->SetByteArrayRegion(array, 0, len, (jbyte *) data);
    double timestamp = (double) encode_byte / (double) (sample * channels * sizeof(short)) * 1000 * 1000;
//    LOGI("audio encode timestamp %f", timestamp);
    env->CallVoidMethod(j_encoder, encode_id, array, (long) timestamp);
    env->DeleteLocalRef(array);
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

void MediaCodecAACEncoder::close() {
    if (j_encoder == nullptr) {
        LOGE("Decode Object is null when open");
        return;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    env->CallVoidMethod(j_encoder, close_id);
    env->DeleteGlobalRef(j_encoder);
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

MediaCodecAACEncoder::~MediaCodecAACEncoder() {
    javaVm = nullptr;
    env = nullptr;

}
