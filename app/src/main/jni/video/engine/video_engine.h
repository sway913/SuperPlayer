//
// Created by Lvlingling on 2020/8/2.
//

#ifndef SUPERPLAYER_VIDEO_ENGINE_H
#define SUPERPLAYER_VIDEO_ENGINE_H


#include <jni.h>
#include "../opengl/gl_view.h"
#include "../source/source.h"
#include "../utils/video_effect.h"

class VideoEngine {

public:

    virtual void init(JNIEnv *env, const std::shared_ptr<GlView> &gl_view) = 0;

    virtual void stop() = 0;

    virtual void switchCamera();

    virtual void setEffect(std::shared_ptr<VideoEffect> &effect);

    virtual void prepare(JNIEnv *env, const char *path) = 0;

    virtual void start() = 0;

    virtual void resume() = 0;

    virtual void pause() = 0;

    virtual void encodeFrame(uint8_t *frame);

    virtual ~VideoEngine();

    static VideoEngine *getVideoEngine(int mode);

protected:

    JNIEnv *env{nullptr};
    std::shared_ptr<GlView> glView{nullptr};

};


#endif //SUPERPLAYER_VIDEO_ENGINE_H
