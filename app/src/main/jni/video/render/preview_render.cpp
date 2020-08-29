//
// Created by Lvlingling on 2020/8/14.
//

#include <zconf.h>
#include "preview_render.h"


void PreviewRender::onSurfaceCreate(jobject surface, int w, int h) {
    source->open(w, h, surface);
}

void PreviewRender::onDraw() {
    source->produceFrame();
}

void PreviewRender::onSurfaceDestroy() {
    source->close();
}

void PreviewRender::setSource(Source *s) {
    this->source = s;
}


void PreviewRender::setState(bool isPause) {

}

void PreviewRender::setFilter(const std::shared_ptr<VFilter> &f) {

}

void PreviewRender::start(JNIEnv *env, const char *video_path) {

}

void PreviewRender::stop() {

}

PreviewRender::~PreviewRender() = default;

