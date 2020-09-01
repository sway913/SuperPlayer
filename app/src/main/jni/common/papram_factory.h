//
// Created by ldr on 2020/6/6.
//

#ifndef SUPERPLAYER_PAPRAM_FACTORY_H
#define SUPERPLAYER_PAPRAM_FACTORY_H

#include <jni.h>
#include "merger_param.h"
#include "audio_param.h"
#include "../video/utils/video_effect.h"

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
        jmethodID is_video_id = env->GetMethodID(cls, "isVideoMode", "()Z");
        jmethodID video_path_id = env->GetMethodID(cls, "getVideoPath", "()Ljava/lang/String;");


        bool is_recorder = env->CallBooleanMethod(obj, is_recorder_id);
        int out_sample = env->CallIntMethod(obj, sample_id);
        auto j_acc_path = static_cast<jstring>(env->CallObjectMethod(obj, acc_path_id));
        auto j_guide_path = static_cast<jstring>(env->CallObjectMethod(obj, guide_path_id));
        auto j_vocal_path = static_cast<jstring>(env->CallObjectMethod(obj, vocal_path_id));
        auto j_decode_path = static_cast<jstring>(env->CallObjectMethod(obj, decode_path_id));
        auto j_video_path = static_cast<jstring>(env->CallObjectMethod(obj, video_path_id));
        const char *acc_path = env->GetStringUTFChars(j_acc_path, nullptr);
        const char *guide_path = env->GetStringUTFChars(j_guide_path, nullptr);
        const char *vocal_path = env->GetStringUTFChars(j_vocal_path, nullptr);
        const char *decode_path = env->GetStringUTFChars(j_decode_path, nullptr);
        const char *video_path = env->GetStringUTFChars(j_video_path, nullptr);
        bool is_video = env->CallBooleanMethod(obj, is_video_id);

        std::shared_ptr<AudioParam> sp_param = std::make_shared<AudioParam>(is_recorder, out_sample, acc_path, guide_path, vocal_path,
                                                                            decode_path, is_video, video_path);
        env->ReleaseStringChars(j_acc_path, reinterpret_cast<const jchar *>(acc_path));
        env->ReleaseStringChars(j_guide_path, reinterpret_cast<const jchar *>(guide_path));
        env->ReleaseStringChars(j_vocal_path, reinterpret_cast<const jchar *>(vocal_path));
        env->ReleaseStringChars(j_decode_path, reinterpret_cast<const jchar *>(decode_path));
        env->ReleaseStringChars(j_video_path, reinterpret_cast<const jchar *>(video_path));
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
        jmethodID mode_id = env->GetMethodID(cls, "getMode", "()I");
        jmethodID video_path_id = env->GetMethodID(cls, "getVideoPath", "()Ljava/lang/String;");
        jmethodID out_video_path_id = env->GetMethodID(cls, "getOutVideoPath", "()Ljava/lang/String;");
        jmethodID is_video_id = env->GetMethodID(cls, "isVideo", "()Z");

        auto j_acc_path = static_cast<jstring>(env->CallObjectMethod(obj, acc_path_id));
        auto j_vocal_path = static_cast<jstring>(env->CallObjectMethod(obj, vocal_path_id));
        auto j_out_path = static_cast<jstring>(env->CallObjectMethod(obj, out_path_id));
        auto j_video_path = static_cast<jstring>(env->CallObjectMethod(obj, video_path_id));
        auto j_out_video_path = static_cast<jstring>(env->CallObjectMethod(obj, out_video_path_id));
        float vocal_vol = env->CallFloatMethod(obj, vocal_vol_id);
        float acc_vol = env->CallFloatMethod(obj, acc_vol_id);
        float pitch = env->CallFloatMethod(obj, pitch_id);
        jfloatArray j_effect = static_cast<jfloatArray>(env->CallObjectMethod(obj, effect_id));
        int mode = env->CallIntMethod(obj, mode_id);
        bool is_video = env->CallBooleanMethod(obj, is_video_id);

        const char *acc_path = env->GetStringUTFChars(j_acc_path, nullptr);
        const char *vocal_path = env->GetStringUTFChars(j_vocal_path, nullptr);
        const char *out_path = env->GetStringUTFChars(j_out_path, nullptr);
        const char *video_path = env->GetStringUTFChars(j_video_path, nullptr);
        const char *out_video_path = env->GetStringUTFChars(j_out_video_path, nullptr);

        float *effect = env->GetFloatArrayElements(j_effect, nullptr);

        std::shared_ptr<MergerParam> sp_param = std::make_shared<MergerParam>(mode, is_video, acc_path, vocal_path,
                                                                              video_path, out_path, out_video_path,
                                                                              vocal_vol, acc_vol, pitch, effect);

        env->ReleaseStringChars(j_acc_path, reinterpret_cast<const jchar *>(acc_path));
        env->ReleaseStringChars(j_vocal_path, reinterpret_cast<const jchar *>(vocal_path));
        env->ReleaseStringChars(j_out_path, reinterpret_cast<const jchar *>(out_path));
        env->ReleaseStringChars(j_video_path, reinterpret_cast<const jchar *>(video_path));
        env->ReleaseStringChars(j_out_video_path, reinterpret_cast<const jchar *>(out_video_path));
        env->ReleaseFloatArrayElements(j_effect, effect, 0);
        env->DeleteLocalRef(cls);
        return sp_param;
    }

    static std::shared_ptr<VideoEffect> generalVideoEffect(JNIEnv *env, jobject obj) {
        jclass cls = env->GetObjectClass(obj);
        jmethodID white_id = env->GetMethodID(cls, "getWhiteLevel", "()F");
        jmethodID smooth_id = env->GetMethodID(cls, "getSmoothLevel", "()F");
        jmethodID path_id = env->GetMethodID(cls, "getLookupTablePath", "()Ljava/lang/String;");

        float white_level = env->CallFloatMethod(obj, white_id);
        float smooth_level = env->CallFloatMethod(obj, smooth_id);
        auto j_path = static_cast<jstring>(env->CallObjectMethod(obj, path_id));
        const char *path = env->GetStringUTFChars(j_path, nullptr);
        std::shared_ptr<VideoEffect> sp_param = std::make_shared<VideoEffect>(white_level, smooth_level, path);
        env->ReleaseStringChars(j_path, (const jchar *) path);
        env->DeleteLocalRef(cls);
        return sp_param;
    }

};

#endif //SUPERPLAYER_PAPRAM_FACTORY_H
