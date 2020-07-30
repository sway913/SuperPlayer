//
// Created by Lvlingling on 2020/6/25.
//

#include "super_video.h"

SuperVideo::SuperVideo(JNIEnv *env) : env(env) {
    env->GetJavaVM(&javaVm);
    glView = new GlView();
    render = new Render(env);
    source = new Camera(env);

    render->setSource(source);
    glView->setRender(render);

    combine_filter = std::make_shared<CombineFilter>();
    render->setFilter(combine_filter);
}

void SuperVideo::createSurface(jobject surface, int width, int height) {
    glView->createSurface(env, surface, width, height);
    source->open(width, height);
    glView->requestRender();
}

void SuperVideo::destroySurface() {
    source->close();
    glView->requestRender();
    glView->destroySurface();
}

void SuperVideo::onFrameAvailable() {
    glView->requestRender();
}

void SuperVideo::switchCamera() {
    if (dynamic_cast<Camera *>(source)) {
        ((Camera *) source)->switchCamera();
    }
}

void SuperVideo::setEffect(std::shared_ptr<VideoEffect> &effect) {
    combine_filter->setEffect(effect);
    LOGI("super video set effect");
}

void SuperVideo::startRecord() {
    render->startRecord();
}

void SuperVideo::stopRecord() {
    render->stopRecord();
}

SuperVideo::~SuperVideo() {
    DELETEOBJ(glView)
    DELETEOBJ(render)
    DELETEOBJ(source)
    combine_filter = nullptr;
}
