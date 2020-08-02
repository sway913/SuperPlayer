//
// Created by Lvlingling on 2020/8/2.
//

#include "video_engine.h"
#include "record_engine.h"
#include "play_engine.h"

VideoEngine *VideoEngine::getVideoEngine(JNIEnv *jniEnv, int mode) {
    if (mode == 0) {
        return new RecordEngine(jniEnv);
    } else {
        return new PlayEngine(jniEnv);
    }
}

VideoEngine::VideoEngine(JNIEnv *env) : env(env) {}

void VideoEngine::setGlView(GlView *v) {
    if (v != nullptr) {
        this->glView = v;
        initGlView();
    }
}

void VideoEngine::switchCamera() {

}

void VideoEngine::setEffect(std::shared_ptr<VideoEffect> &effect) {

}


VideoEngine::~VideoEngine() = default;
