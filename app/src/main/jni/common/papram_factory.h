//
// Created by ldr on 2020/6/6.
//

#ifndef SUPERPLAYER_PAPRAM_FACTORY_H
#define SUPERPLAYER_PAPRAM_FACTORY_H

#include <jni.h>
#include "merger_param.h"
#include "audio_param.h"

class ParamFactory {

public:

    static std::shared_ptr<AudioParam> generalAudioParam(JNIEnv *env, jobject obj) {
        jclass cls = env->GetObjectClass(obj);
        jmethodID is_recorder_id = env->GetMethodID(cls, "isRecorder", "()Z");
        jmethodID sample_id = env->GetMethodID(cls, "getOutSample", "()I");
        jmethodID acc_path_id = env->GetMethodID(cls, "getAccPath", "()Ljava/lang/String;");
        jmethodID vocal_path_id = env->GetMethodID(cls, "getVocalPath", "()Ljava/lang/String;");
        jmethodID guide_path_id = env->GetMethodID(cls, "getGuidePath", "()Ljava/lang/String;");
        jmethodID decode_path_id = env->GetMethodID(cls, "getDecodePath", "()Ljava/lang/String;");

        bool is_recorder = env->CallBooleanMethod(obj, is_recorder_id);
        int out_sample = env->CallIntMethod(obj, sample_id);
        auto j_acc_path = static_cast<jstring>(env->CallObjectMethod(obj, acc_path_id));
        auto j_guide_path = static_cast<jstring>(env->CallObjectMethod(obj, guide_path_id));
        auto j_vocal_path = static_cast<jstring>(env->CallObjectMethod(obj, vocal_path_id));
        auto j_decode_path = static_cast<jstring>(env->CallObjectMethod(obj, decode_path_id));
        const char *acc_path = env->GetStringUTFChars(j_acc_path, nullptr);
        const char *guide_path = env->GetStringUTFChars(j_guide_path, nullptr);
        const char *vocal_path = env->GetStringUTFChars(j_vocal_path, nullptr);
        const char *decode_path = env->GetStringUTFChars(j_decode_path, nullptr);
        std::shared_ptr<AudioParam> sp_param = std::make_shared<AudioParam>(is_recorder, out_sample, acc_path, guide_path, vocal_path, decode_path);
        env->ReleaseStringChars(j_acc_path, reinterpret_cast<const jchar *>(acc_path));
        env->ReleaseStringChars(j_guide_path, reinterpret_cast<const jchar *>(guide_path));
        env->ReleaseStringChars(j_vocal_path, reinterpret_cast<const jchar *>(vocal_path));
        env->ReleaseStringChars(j_decode_path, reinterpret_cast<const jchar *>(decode_path));
        env->DeleteLocalRef(cls);
        return sp_param;
    }


    static std::shared_ptr<MergerParam> generalMergerParam(JNIEnv *env, jobject obj) {
        jclass cls = env->GetObjectClass(obj);
        jmethodID acc_path_id = env->GetMethodID(cls, "getAccPath", "()Ljava/lang/String;");
        jmethodID vocal_path_id = env->GetMethodID(cls, "getVocalPath", "()Ljava/lang/String;");
        jmethodID out_path_id = env->GetMethodID(cls, "getOutPath", "()Ljava/lang/String;");
        jmethodID vocal_vol_id = env->GetMethodID(cls, "getVocalVolume", "()F");
        jmethodID acc_vol_id = env->GetMethodID(cls, "getAccVolume", "()F");
        jmethodID pitch_id = env->GetMethodID(cls, "getPitch", "()F");
        jmethodID effect_id = env->GetMethodID(cls, "getEffect", "()[F");

        auto j_acc_path = static_cast<jstring>(env->CallObjectMethod(obj, acc_path_id));
        auto j_vocal_path = static_cast<jstring>(env->CallObjectMethod(obj, vocal_path_id));
        auto j_out_path = static_cast<jstring>(env->CallObjectMethod(obj, out_path_id));
        float vocal_vol = env->CallFloatMethod(obj, vocal_vol_id);
        float acc_vol = env->CallFloatMethod(obj, acc_vol_id);
        float pitch = env->CallFloatMethod(obj, pitch_id);
        jfloatArray j_effect = static_cast<jfloatArray>(env->CallObjectMethod(obj, effect_id));

        const char *acc_path = env->GetStringUTFChars(j_acc_path, nullptr);
        const char *vocal_path = env->GetStringUTFChars(j_vocal_path, nullptr);
        const char *out_path = env->GetStringUTFChars(j_out_path, nullptr);

        float *effect = env->GetFloatArrayElements(j_effect, nullptr);

        std::shared_ptr<MergerParam> sp_param = std::make_shared<MergerParam>(acc_path, vocal_path, out_path, vocal_vol, acc_vol, pitch, effect);

        env->ReleaseStringChars(j_acc_path, reinterpret_cast<const jchar *>(acc_path));
        env->ReleaseStringChars(j_vocal_path, reinterpret_cast<const jchar *>(vocal_path));
        env->ReleaseStringChars(j_out_path, reinterpret_cast<const jchar *>(out_path));
        env->ReleaseFloatArrayElements(j_effect, effect, 0);
        env->DeleteLocalRef(cls);
        return sp_param;
    }

};

#endif //SUPERPLAYER_PAPRAM_FACTORY_H
