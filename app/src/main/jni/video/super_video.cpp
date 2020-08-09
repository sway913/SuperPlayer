//
// Created by Lvlingling on 2020/6/25.
//

#include "super_video.h"

SuperVideo::SuperVideo(JNIEnv *env, int mode) {
    videoEngine = VideoEngine::getVideoEngine(env, mode);

}

void SuperVideo::prepare(JNIEnv *env, const char *path) {
    videoEngine->prepare(env, path);
}

void SuperVideo::setGlView(GlView *v) {
    videoEngine->setGlView(v);
}

void SuperVideo::switchCamera() {
    videoEngine->switchCamera();
}

void SuperVideo::setEffect(std::shared_ptr<VideoEffect> &effect) {
    videoEngine->setEffect(effect);
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
    videoEngine->setGlView(nullptr);
    DELETEOBJ(videoEngine)
    LOGI("~ Super_Video");
}
