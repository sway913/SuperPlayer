//
// Created by Lvlingling on 2020/6/25.
//

#include "super_video.h"

SuperVideo::SuperVideo(JNIEnv *env) : env(env) {
    env->GetJavaVM(&javaVm);
    glView = new GlView();
    render = new Render();
    source = new Camera(env);

    render->setSource(source);
    glView->setRender(render);
}

void SuperVideo::createSurface(jobject surface, int width, int height) {
    source->open(width, height);
    glView->createSurface(env, surface, width, height);
}

void SuperVideo::destroySurface() {
    source->close();
    glView->destroySurface();
}

void SuperVideo::onFrameAvailable() {
    glView->requestRender();
}

SuperVideo::~SuperVideo() {
    DELETEOBJ(glView)
    DELETEOBJ(render)
    DELETEOBJ(source)
}
