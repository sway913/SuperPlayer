//
// Created by Lvlingling on 2020/6/25.
//

#include "super_video.h"

SuperVideo::SuperVideo(int mode) {
    videoEngine = VideoEngine::getVideoEngine(mode);
}

void SuperVideo::init(JNIEnv *env, const std::shared_ptr<GlView> &gl_view) {
    if (!initialize) {
        initialize = true;
        videoEngine->init(env, gl_view);
    }
}

void SuperVideo::prepare(JNIEnv *env, const char *path) {
    videoEngine->prepare(env, path);
}

void SuperVideo::switchCamera() {
    videoEngine->switchCamera();
}

void SuperVideo::setEffect(std::shared_ptr<VideoEffect> &effect) {
    videoEngine->setEffect(effect);
}

void SuperVideo::start() {
    videoEngine->start();
}

void SuperVideo::resume() {
    videoEngine->resume();
}

void SuperVideo::pause() {
    videoEngine->pause();
}

void SuperVideo::stop() {
    videoEngine->stop();
}

SuperVideo::~SuperVideo() {
    DELETEOBJ(videoEngine)
    LOGI("~ Super_Video");
}
