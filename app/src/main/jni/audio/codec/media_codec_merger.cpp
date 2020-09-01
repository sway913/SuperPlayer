//
// Created by Lvlingling on 2020/9/1.
//

#include "media_codec_merger.h"
#include "../../../../../../../../Library/Android/sdk/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/jni.h"
#include "../../common/android_log.h"

MediaCodecMerger::MediaCodecMerger(JNIEnv *e) {
    e->GetJavaVM(&javaVm);
    this->env = e;
    jclass clazz = env->FindClass("com/smzh/superplayer/video/H264_AAC_toMp4_MediaMuxer");
    if (clazz == nullptr) {
        LOGI("video source not find video decoder");
    }
    jmethodID general_id = env->GetMethodID(clazz, "<init>", "()V");
    if (general_id == nullptr) {
        LOGI("video source not find video decoder init");
    }
    jobject merger = env->NewObject(clazz, general_id);
    j_merger = env->NewGlobalRef(merger);

    start_id = env->GetMethodID(clazz, "mergeVideo", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(merger);
}


void MediaCodecMerger::start(const char *aacPath, const char *h264Path, const char *outPath) {
    if (j_merger == nullptr) {
        LOGE("Decode Object is null when open");
        return;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    jstring acc_path = env->NewStringUTF(aacPath);
    jstring h264_path = env->NewStringUTF(h264Path);
    jstring out_path = env->NewStringUTF(outPath);
    env->CallVoidMethod(j_merger, start_id, acc_path, h264_path, out_path);
    env->DeleteGlobalRef(j_merger);
    env->DeleteLocalRef(acc_path);
    env->DeleteLocalRef(h264_path);
    env->DeleteLocalRef(out_path);
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

