//
// Created by Lvlingling on 2020/6/25.
//

#include "super_video.h"

SuperVideo::SuperVideo(JNIEnv *env) : env(env) {
    glView = new GlView();
    videoEngine = VideoEngine::getVideoEngine(env, 0);
    videoEngine->setGlView(glView);
}

void SuperVideo::prepare(JNIEnv *env, const char *path) {
    videoEngine->prepare(env, path);
}

void SuperVideo::createSurface(jobject surface, int width, int height) {
    glView->createSurface(env, surface, width, height);
    glView->requestRender();
}

void SuperVideo::destroySurface() {
    glView->requestRender();
    glView->destroySurface();
}

void SuperVideo::onFrameAvailable() {
    glView->requestRender();
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
    DELETEOBJ(glView)
    DELETEOBJ(videoEngine)
    LOGI("~ Super_Video");
}
