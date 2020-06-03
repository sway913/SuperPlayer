#include <jni.h>
#include <memory>
#include "audio/super_audio.h"

extern "C" {

std::unique_ptr<SuperAudio> super_audio = nullptr;

JNIEXPORT void JNICALL
Java_com_smzh_superplayer_player_PlayerJni_prepare(JNIEnv *env, jobject clazz) {
    if (super_audio) {
        super_audio->prepare();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_start(JNIEnv *jniEnv, jobject clazz) {
    if (super_audio) {
        super_audio->start();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_resume(JNIEnv *jniEnv, jobject clazz) {
    if (super_audio) {
        super_audio->resume();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_pause(JNIEnv *jniEnv, jobject clazz) {
    if (super_audio) {
        super_audio->pause();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_stop(JNIEnv *env, jobject clazz) {
    if (super_audio) {
        super_audio->stop();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setEcho(JNIEnv *env, jobject clazz, jboolean isEcho) {
    if (super_audio) {
        super_audio->setEcho(isEcho);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setFilter(JNIEnv *env, jobject clazz, jint type) {
    if (super_audio) {
        super_audio->setFilter(type);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setPitch(JNIEnv *env, jobject clazz, jint pitch) {
    if (super_audio) {
        super_audio->setPitch(pitch);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setVolume(JNIEnv *env, jobject clazz, jfloat volume, jint track) {
    if (super_audio) {
        super_audio->setVolume(volume, track);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_seek(JNIEnv *env, jobject clazz, jlong millis) {
    if (super_audio) {
        super_audio->seek(millis);
    }
}

JNIEXPORT jlong JNICALL Java_com_smzh_superplayer_player_PlayerJni_getTotalMs(JNIEnv *env, jobject clazz) {
    if (super_audio) {
        return super_audio->getTotalMs();
    }
    return 0;
}

JNIEXPORT jlong JNICALL Java_com_smzh_superplayer_player_PlayerJni_getCurrentMs(JNIEnv *env, jobject clazz) {
    if (super_audio) {
        super_audio->getCurrentMs();
    }
    return 0;
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_startMerge(JNIEnv *env, jobject clazz) {
    if (super_audio) {

    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_stopMerge(JNIEnv *env, jobject clazz) {
    if (super_audio) {

    }
}

JNIEXPORT jint JNICALL Java_com_smzh_superplayer_player_PlayerJni_getMergeProgress(JNIEnv *env, jobject clazz) {
    if (super_audio) {

    }
    return 0;
}
}

