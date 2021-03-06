//
// Created by Lvlingling on 2020/8/2.
//

#include "record_engine.h"
#include "../source/camera.h"


void RecordEngine::init(JNIEnv *env, const std::shared_ptr<GlView> &gl_view) {
    glView = gl_view;
    source = new Camera(env);
    combine_filter = std::make_shared<CombineFilter>();
    render = new CameraRender();
    render->setFilter(combine_filter);
    render->setSource(source);
    glView->setRender(render);
}

void RecordEngine::prepare(JNIEnv *env, const char *path) {
    render->start(env, path);
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

void RecordEngine::start() {
    resume();
}

void RecordEngine::resume() {
    render->setState(false);
}

void RecordEngine::pause() {
    render->setState(true);
}

void RecordEngine::encodeFrame(uint8_t *frame) {
    if (render) {
        render->encodeFrame(frame);
    }
}

RecordEngine::~RecordEngine() {
    DELETEOBJ(source)
    DELETEOBJ(render)
    combine_filter = nullptr;
}
