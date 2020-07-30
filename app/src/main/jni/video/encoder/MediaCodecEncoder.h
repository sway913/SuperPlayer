//
// Created by ldr on 2020/7/16.
//

#ifndef SUPERPLAYER_MEDIACODECENCODER_H
#define SUPERPLAYER_MEDIACODECENCODER_H


#include "VideoEncoder.h"

class MediaCodecEncoder : public VideoEncoder {

public:

    MediaCodecEncoder(JNIEnv *env);

    void start() override;

    void encodeFrame() override;

    void stop() override;

private:

    JavaVM *javaVm{nullptr};
    jobject video_encoder{nullptr};
    jmethodID encode_id{nullptr};
    jmethodID stop_id{nullptr};
};


#endif //SUPERPLAYER_MEDIACODECENCODER_H
