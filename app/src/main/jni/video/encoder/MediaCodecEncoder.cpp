//
// Created by ldr on 2020/7/16.
//

#include "MediaCodecEncoder.h"
#include "../../common/android_log.h"


MediaCodecEncoder::MediaCodecEncoder(JNIEnv *env, int w, int h, const char *path) {
    if (env) {
        env->GetJavaVM(&javaVm);
        LOGI("mediaCodec path %s", path);
        jclass cls = env->FindClass("com/smzh/superplayer/video/VideoEncoder");
        jmethodID init_id = env->GetMethodID(cls, "<init>", "(IIIILjava/lang/String;)V");
        jstring str = env->NewStringUTF(path);
        jobject encoder = env->NewObject(cls, init_id, w, h, 4000000, 30, str);
        video_encoder = env->NewGlobalRef(encoder);
        encode_id = env->GetMethodID(cls, "encodeFrame", "()V");
        stop_id = env->GetMethodID(cls, "stopEncoder", "()V");
        get_surface_id = env->GetMethodID(cls, "getInputSurface", "()Landroid/view/Surface;");
        jobject surface = env->CallObjectMethod(video_encoder, get_surface_id);
        if (surface) {
            encodeWindow = ANativeWindow_fromSurface(env, surface);
        }
    }
}

void MediaCodecEncoder::stop() {
    JNIEnv *env = nullptr;
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    env->CallVoidMethod(video_encoder, stop_id);
    if (encodeWindow) {
        ANativeWindow_release(encodeWindow);
    }
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

void MediaCodecEncoder::encodeFrame(uint8_t *data) {
    JNIEnv *env = nullptr;
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    env->CallVoidMethod(video_encoder, encode_id);
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

ANativeWindow *MediaCodecEncoder::getEncodeWindow() {
    return encodeWindow;
}