#include <jni.h>
#include <memory>
#include "audio/super_audio.h"
#include "common/audio_param.h"
#include "common/papram_factory.h"
#include "audio/merger/audio_merger2.h"
#include "common/merger_param.h"

extern "C" {

std::unique_ptr<SuperAudio> super_audio = nullptr;
std::unique_ptr<AudioMerger2> audio_merger = nullptr;

JNIEXPORT void JNICALL
Java_com_smzh_superplayer_player_PlayerJni_prepare(JNIEnv *env, jobject clazz, jobject audio_param) {
    super_audio = std::make_unique<SuperAudio>(env, clazz);
    if (super_audio) {
        std::shared_ptr<AudioParam> spParam = ParamFactory::generalAudioParam(env, audio_param);
        super_audio->prepare(spParam);
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
    super_audio = nullptr;
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setEcho(JNIEnv *env, jobject clazz, jboolean isEcho) {
    if (super_audio) {
        super_audio->setEcho(isEcho);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setFilter(JNIEnv *env, jobject clazz, jfloatArray array) {
    if (super_audio) {
        jfloat *param = env->GetFloatArrayElements(array, nullptr);
        super_audio->setFilter(param);
        env->ReleaseFloatArrayElements(array, param, 0);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setPitch(JNIEnv *env, jobject clazz, jfloat pitch) {
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
        return super_audio->getCurrentMs();
    }
    return 0;
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_startMerge(JNIEnv *env, jobject clazz, jobject param) {
    audio_merger = std::make_unique<AudioMerger2>();
    std::shared_ptr<MergerParam> sp_param = ParamFactory::generalMergerParam(env, param);
    audio_merger->start(sp_param);
}

JNIEXPORT jint JNICALL Java_com_smzh_superplayer_player_PlayerJni_getMergeProgress(JNIEnv *env, jobject clazz) {
    if (audio_merger) {
        return audio_merger->getProgress();
    }
    return 0;
}
}

