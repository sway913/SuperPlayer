//
// Created by Lvlingling on 2019-11-04.
//
#include <jni.h>

extern "C" {


JNIEXPORT void JNICALL
Java_com_smzh_superplayer_AudioJni_prepare(JNIEnv *env, jobject clazz, jboolean isRecordMode, jint sample, jstring AudioPath, jstring guidePath,
                                           jstring vocalPath) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_start(JNIEnv *jniEnv, jobject clazz) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_resume(JNIEnv *jniEnv, jobject clazz) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_pause(JNIEnv *jniEnv, jobject clazz) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_stop(JNIEnv *env, jobject clazz) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_setEcho(JNIEnv *env, jobject clazz, jboolean isEcho) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_setFilter(JNIEnv *env, jobject clazz, jint type) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_setPitch(JNIEnv *env, jobject clazz, jint pitch) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_setVolume(JNIEnv *env, jobject clazz, jfloat volume, jint track) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_seek(JNIEnv *env, jobject clazz, jlong millis) {

}

JNIEXPORT jlong JNICALL Java_com_smzh_superplayer_AudioJni_getTotalMs(JNIEnv *env, jobject clazz) {
    return 0;
}

JNIEXPORT jlong JNICALL Java_com_smzh_superplayer_AudioJni_getCurrentMs(JNIEnv *env, jobject clazz) {
    return 0;
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_prepareMerge(JNIEnv *env, jobject clazz, jstring vocalPath, jstring musicPath,
                                                                       jstring mixPath) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_startMerge(JNIEnv *env, jobject clazz, jint vocalVolume, jint vocalFilter, jint musicVolume, jfloat musicPitch) {

}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_AudioJni_stopMerge(JNIEnv *env, jobject clazz) {

}

JNIEXPORT jint JNICALL Java_com_smzh_superplayer_AudioJni_getMergeProgress(JNIEnv *env, jobject clazz) {
    return 0;
}
}

