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

    SuperVideo(int mode);

    void init(JNIEnv *env, const std::shared_ptr<GlView> &gl_view);

    void prepare(JNIEnv *env, const char *path);

    void switchCamera();

    void setEffect(std::shared_ptr<VideoEffect> &effect);

    void resume();

    void pause();

    void stop();

    virtual ~SuperVideo();

private:

    VideoEngine *videoEngine{nullptr};
    bool initialize{false};
    const char *video_path{nullptr};

};


#endif //SUPERPLAYER_SUPER_VIDEO_H
