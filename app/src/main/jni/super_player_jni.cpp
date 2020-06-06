#include <jni.h>
#include <memory>
#include "audio/super_audio.h"
#include "common/audio_param.h"

extern "C" {

std::unique_ptr<SuperAudio> super_audio = nullptr;

JNIEXPORT void JNICALL
Java_com_smzh_superplayer_player_PlayerJni_prepare(JNIEnv *env, jobject clazz, jobject audio_param) {
    super_audio = std::make_unique<SuperAudio>(env, clazz);
    if (super_audio) {
        jclass cls = env->GetObjectClass(audio_param);
        jmethodID is_recorder_id = env->GetMethodID(cls, "isRecorder", "()Z");
        jmethodID sample_id = env->GetMethodID(cls, "getOutSample", "()I");
        jmethodID acc_id = env->GetMethodID(cls, "getAccPath", "()Ljava/lang/String;");
        jmethodID vocal_id = env->GetMethodID(cls, "getVocalPath", "()Ljava/lang/String;");
        jmethodID fun_id = env->GetMethodID(cls, "getFunctionPath", "()Ljava/lang/String;");

        bool is_recorder = env->CallBooleanMethod(audio_param, is_recorder_id);
        int out_sample = env->CallIntMethod(audio_param, sample_id);
        auto j_acc = static_cast<jstring>(env->CallObjectMethod(audio_param, acc_id));
        auto j_fun = static_cast<jstring>(env->CallObjectMethod(audio_param, fun_id));
        auto j_vocal = static_cast<jstring>(env->CallObjectMethod(audio_param, vocal_id));
        const char *acc_path = env->GetStringUTFChars(j_acc, nullptr);
        const char *fun_path = env->GetStringUTFChars(j_fun, nullptr);
        const char *vocal_path = env->GetStringUTFChars(j_vocal, nullptr);
        std::shared_ptr<AudioParam> spParam = std::make_shared<AudioParam>(is_recorder, acc_path, fun_path, vocal_path, out_sample);
        super_audio->prepare(spParam);
        env->ReleaseStringChars(j_acc, reinterpret_cast<const jchar *>(acc_path));
        env->ReleaseStringChars(j_fun, reinterpret_cast<const jchar *>(fun_path));
        env->ReleaseStringChars(j_vocal, reinterpret_cast<const jchar *>(vocal_path));
        env->DeleteLocalRef(cls);
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

