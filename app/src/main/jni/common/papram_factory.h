//
// Created by ldr on 2020/6/6.
//

#ifndef SUPERPLAYER_PAPRAM_FACTORY_H
#define SUPERPLAYER_PAPRAM_FACTORY_H

#include <jni.h>

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

};

#endif //SUPERPLAYER_PAPRAM_FACTORY_H
