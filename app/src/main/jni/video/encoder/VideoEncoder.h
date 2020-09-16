//
// Created by ldr on 2020/7/16.
//

#ifndef SUPERPLAYER_VIDEOENCODER_H
#define SUPERPLAYER_VIDEOENCODER_H

#include <jni.h>

class VideoEncoder {

public:

    static VideoEncoder *getEncoder(JNIEnv *env, const char *path, int w, int h, bool is_support_media_codec = true);

    virtual void encodeFrame(uint8_t *data = nullptr) = 0;

    virtual void stop() = 0;

    virtual ~VideoEncoder();
};

#endif //SUPERPLAYER_VIDEOENCODER_H
