//
// Created by Lvlingling on 2020/8/11.
//

#include "video_source.h"
#include "../../common/android_log.h"
#include <unistd.h>

VideoSource::VideoSource(JNIEnv *env, std::function<void(void)> cb) : env(env) {
}

void VideoSource::prepare(JNIEnv *e, const char *p) {
    std::lock_guard<std::mutex> lock(mutex_);
    e->GetJavaVM(&javaVm);
    this->env = e;
    this->path = p;
    jclass clazz = env->FindClass("com/smzh/superplayer/video/VideoDecoder");
    if (clazz == nullptr) {
        LOGI("video source not find video decoder");
    }
    jmethodID general_id = env->GetMethodID(clazz, "<init>", "()V");
    if (general_id == nullptr) {
        LOGI("video source not find video decoder init");
    }
    jobject decoder = env->NewObject(clazz, general_id);
    j_decoder = env->NewGlobalRef(decoder);

    close_id = env->GetMethodID(clazz, "release", "()V");
    init_id = env->GetMethodID(clazz, "init", "(Ljava/lang/String;Landroid/view/Surface;)V");
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(decoder);
}


void VideoSource::open(int w, int h, jobject surface) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (j_decoder == nullptr) {
        LOGE("Decode Object is null when open");
        return;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }

    if (path != nullptr) {
        LOGI("video path %s", path);
    }

    jstring p = env->NewStringUTF(path);
    env->CallVoidMethod(j_decoder, init_id, p, surface);
    env->DeleteLocalRef(p);
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

VideoFrame *VideoSource::produceFrame() {
    return nullptr;
}

void VideoSource::close() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (j_decoder == nullptr) {
        return;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    env->CallVoidMethod(j_decoder, close_id);
    env->DeleteGlobalRef(j_decoder);
    j_decoder = nullptr;
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

void VideoSource::stop() {
    close();
}

VideoSource::~VideoSource() = default;
