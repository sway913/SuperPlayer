//
// Created by Lvlingling on 2020/9/1.
//

#ifndef SUPERPLAYER_MEDIA_CODEC_AAC_ENCODER_H
#define SUPERPLAYER_MEDIA_CODEC_AAC_ENCODER_H


#include <jni.h>
#include "i_audio_encoder.h"

class MediaCodecAACEncoder : public IAudioEncoder {

public:

    MediaCodecAACEncoder(JNIEnv *env);

    void init(int in_sample, int in_channels, const char *file_name) override;

    void encode(short *data, int len) override;

    void close() override;

    virtual ~MediaCodecAACEncoder();

private:

    JNIEnv *env{nullptr};
    JavaVM *javaVm{nullptr};
    jobject j_encoder{nullptr};
    jmethodID close_id{nullptr};
    jmethodID init_id{nullptr};
    jmethodID encode_id{nullptr};
    int64_t encode_byte{0};
    int sample{0};
    int channels{0};
};


#endif //SUPERPLAYER_MEDIA_CODEC_AAC_ENCODER_H
