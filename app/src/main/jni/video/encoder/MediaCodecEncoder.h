//
// Created by ldr on 2020/7/16.
//

#ifndef SUPERPLAYER_MEDIACODECENCODER_H
#define SUPERPLAYER_MEDIACODECENCODER_H


#include "VideoEncoder.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>

class MediaCodecEncoder : public VideoEncoder {

public:

    MediaCodecEncoder(JNIEnv *env, int w, int h, const char *path);

    void encodeFrame(uint8_t *data) override;

    void stop() override;

    ANativeWindow *getEncodeWindow();

private:

    JavaVM *javaVm{nullptr};
    jobject video_encoder{nullptr};
    jmethodID encode_id{nullptr};
    jmethodID stop_id{nullptr};
    ANativeWindow *encodeWindow{nullptr};
    jmethodID get_surface_id{nullptr};
};


#endif //SUPERPLAYER_MEDIACODECENCODER_H
