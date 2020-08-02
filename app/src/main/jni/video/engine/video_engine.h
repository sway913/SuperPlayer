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

    VideoEngine(JNIEnv *env);

    void setGlView(GlView *v);

    virtual void stop() = 0;

    virtual void switchCamera();

    virtual void setEffect(std::shared_ptr<VideoEffect> &effect);

    virtual void prepare(JNIEnv *env, const char *path) = 0;

    virtual void resume() = 0;

    virtual void pause() = 0;

    virtual ~VideoEngine();

    static VideoEngine *getVideoEngine(JNIEnv *jniEnv, int mode);

protected:

    virtual void initGlView() = 0;

    JNIEnv *env{nullptr};
    GlView *glView{nullptr};


};


#endif //SUPERPLAYER_VIDEO_ENGINE_H
