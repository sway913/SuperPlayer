//
// Created by Lvlingling on 2020/8/2.
//

#include "play_engine.h"
#include "../source/video_source.h"
#include "../render/preview_render.h"

void PlayEngine::init(JNIEnv *env, const std::shared_ptr<GlView> &gl_view) {
    glView = gl_view;
    glView->setRender(render);
}

void PlayEngine::prepare(JNIEnv *env, const char *path) {
    render = new PreviewRender();
    source = new VideoSource(env, nullptr);
    render->setSource(source);
    ((VideoSource *) source)->prepare(env, path);
}

void PlayEngine::resume() {

}

void PlayEngine::pause() {

}

void PlayEngine::stop() {
    if (auto *s = dynamic_cast<VideoSource *>(source)) {
        s->stop();
    }
}

PlayEngine::~PlayEngine() {
    DELETEOBJ(source)
}
