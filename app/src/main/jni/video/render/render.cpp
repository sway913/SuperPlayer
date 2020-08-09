//
// Created by Lvlingling on 2020/6/25.
//

#include "render.h"
#include "../../common/common_tools.h"
#include "../../common/android_log.h"

Render::Render() {
    movieWriterFilter = new MovieWriterFilter();
}


void Render::onSurfaceCreate(int w, int h) {
    this->width = w;
    this->height = h;
    if (source) {
        source->open(w, h);
    }
}

void Render::onDraw() {
    GLuint textureId;
    if (source) {
        textureId = source->produceFrame();
        if (filter) {
            textureId = filter->draw(textureId, width, height);
        }
        movieWriterFilter->draw(textureId, width, height);
    }
}


void Render::onSurfaceDestroy() {
    if (source) {
        source->close();
    }
    movieWriterFilter->destroy();
    if (filter) {
        filter->destroy();
        filter = nullptr;
    }
}

void Render::setSource(Source *s) {
    this->source = s;
}

void Render::setFilter(const std::shared_ptr<VFilter> &f) {
    this->filter = f;
}

void Render::start(JNIEnv *env, const char *video_path) {
    movieWriterFilter->startRecord(env, video_path);
}

void Render::stop() {
    movieWriterFilter->stopRecord();
}

void Render::setState(bool isPause) {
    movieWriterFilter->setState(isPause);
}

Render::~Render() {
    DELETEOBJ(movieWriterFilter)
}
