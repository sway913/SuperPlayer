//
// Created by Lvlingling on 2020/8/2.
//

#include "record_engine.h"
#include "../source/camera.h"


RecordEngine::RecordEngine(JNIEnv *env) : VideoEngine(env) {
    render = new Render();
    source = new Camera(env);
    combine_filter = std::make_shared<CombineFilter>();
    render->setFilter(combine_filter);
    render->setSource(source);
}

void RecordEngine::prepare(JNIEnv *env, const char *path) {
    render->start(env, path);
}

void RecordEngine::initGlView() {
    glView->setRender(render);
}

void RecordEngine::stop() {
    render->stop();
}

void RecordEngine::switchCamera() {
    if (dynamic_cast<Camera *>(source)) {
        ((Camera *) source)->switchCamera();
    }
}

void RecordEngine::setEffect(std::shared_ptr<VideoEffect> &effect) {
    combine_filter->setEffect(effect);
}

void RecordEngine::resume() {
    render->resume();
}

void RecordEngine::pause() {
    render->pause();
}

RecordEngine::~RecordEngine() {
    DELETEOBJ(source)
    DELETEOBJ(render);
    combine_filter = nullptr;
}
