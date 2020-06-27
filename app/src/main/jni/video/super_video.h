//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_SUPER_VIDEO_H
#define SUPERPLAYER_SUPER_VIDEO_H


#include <jni.h>
#include "gl_view.h"
#include "../common/common_tools.h"
#include "render.h"
#include "source/camera.h"
#include "source/test_source.h"

class SuperVideo {

public:

    SuperVideo(JNIEnv *env);

    void createSurface(jobject surface, int width, int height);

    void destroySurface();

    void onFrameAvailable();

    virtual ~SuperVideo();

private:

    GlView *glView{nullptr};
    Render *render{nullptr};
    Source *source{nullptr};
    JNIEnv *env{nullptr};
    JavaVM *javaVm{nullptr};

};


#endif //SUPERPLAYER_SUPER_VIDEO_H
