//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_SUPER_VIDEO_H
#define SUPERPLAYER_SUPER_VIDEO_H


#include <jni.h>
#include "../common/common_tools.h"
#include "utils/video_effect.h"
#include "filter/combine_filter.h"
#include "opengl/gl_view.h"
#include "engine/video_engine.h"

class SuperVideo {

public:

    SuperVideo(JNIEnv *env);

    void createSurface(jobject surface, int width, int height);

    void destroySurface();

    void onFrameAvailable();

    void switchCamera();

    void setEffect(std::shared_ptr<VideoEffect> &effect);

    void startRecord();

    void stopRecord();

    virtual ~SuperVideo();

private:

    VideoEngine *videoEngine{nullptr};
    GlView *glView{nullptr};
    JNIEnv *env{nullptr};
};


#endif //SUPERPLAYER_SUPER_VIDEO_H
