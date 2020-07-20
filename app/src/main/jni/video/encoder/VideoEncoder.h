//
// Created by ldr on 2020/7/16.
//

#ifndef SUPERPLAYER_VIDEOENCODER_H
#define SUPERPLAYER_VIDEOENCODER_H

#include <jni.h>

class VideoEncoder {

public:

    static VideoEncoder *getEncoder(JNIEnv *env, bool is_support_media_codec = true);

    virtual void start() = 0;

    virtual void resume() = 0;

    virtual void encodeFrame() = 0;

    virtual void pause() = 0;

    virtual void stop() = 0;
};

#endif //SUPERPLAYER_VIDEOENCODER_H
