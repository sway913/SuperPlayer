//
// Created by Lvlingling on 2020/6/25.
//

#include "camera_render.h"
#include "../../common/common_tools.h"
#include "../../common/android_log.h"

CameraRender::CameraRender() {
    movieWriterFilter = new MovieWriterFilter();
}

void CameraRender::onSurfaceCreate(jobject surface, int w, int h) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->width = w;
    this->height = h;
    if (source) {
        source->open(w, h);
    }
}

void CameraRender::onDraw() {
    if (source) {
        VideoFrame *frame = source->produceFrame();
        if (filter) {
            filter->draw(frame);
        }
        movieWriterFilter->draw(frame);
        delete frame;
    }
}


void CameraRender::onSurfaceDestroy() {
    if (source) {
        source->close();
    }
    movieWriterFilter->destroy();
    if (filter) {
        filter->destroy();
    }
}

void CameraRender::setSource(Source *s) {
    this->source = s;
}

void CameraRender::setFilter(const std::shared_ptr<VFilter> &f) {
    this->filter = f;
}

void CameraRender::start(JNIEnv *env, const char *video_path) {
    movieWriterFilter->startRecord(env, video_path);
}

void CameraRender::stop() {
    movieWriterFilter->stopRecord();
}

void CameraRender::setState(bool isPause) {
    movieWriterFilter->setState(isPause);
}

CameraRender::~CameraRender() {
    DELETEOBJ(movieWriterFilter)
}
