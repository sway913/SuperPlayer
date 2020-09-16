//
// Created by Lvlingling on 2020/6/25.
//

#include "super_video.h"

SuperVideo::SuperVideo(int mode) {
    videoEngine = VideoEngine::getVideoEngine(mode);
    this->mode = mode;
}

void SuperVideo::init(JNIEnv *env, const std::shared_ptr<GlView> &gl_view) {
    if (!initialize) {
        initialize = true;
        videoEngine->init(env, gl_view);
    }
}

void SuperVideo::prepare(JNIEnv *env, const char *path) {
    videoEngine->prepare(env, path);
    if (mode == 0) {
        videoEncoder = VideoEncoder::getEncoder(env, "/storage/emulated/0/000/test_video.mp4", 1080, 1920, false);
    }
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
    if (videoEncoder) {
        videoEncoder->stop();
    }
}

void SuperVideo::encodeFrame(uint8_t *frame) {
    if (videoEncoder) {
        videoEncoder->encodeFrame(frame);
    }
}

SuperVideo::~SuperVideo() {
    DELETEOBJ(videoEngine)
    DELETEOBJ(videoEncoder)
    LOGI("~ Super_Video");
}
