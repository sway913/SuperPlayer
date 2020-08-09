//
// Created by Lvlingling on 2020/8/2.
//

#include "video_engine.h"
#include "record_engine.h"
#include "play_engine.h"

VideoEngine *VideoEngine::getVideoEngine(JNIEnv *jniEnv, const std::shared_ptr<GlView> &gl_view, int mode) {
    if (mode == 0) {
        return new RecordEngine(jniEnv, gl_view);
    } else {
        return new PlayEngine(jniEnv, gl_view);
    }
}

VideoEngine::VideoEngine(JNIEnv *env, const std::shared_ptr<GlView> &gl_view) : env(env), glView(gl_view) {}


void VideoEngine::switchCamera() {

}

void VideoEngine::setEffect(std::shared_ptr<VideoEffect> &effect) {

}


VideoEngine::~VideoEngine() {
    glView = nullptr;
    LOGI("~ Video Engine");
};
