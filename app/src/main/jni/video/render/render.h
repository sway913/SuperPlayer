//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_RENDER_H
#define SUPERPLAYER_RENDER_H


#include <memory>
#include <jni.h>
#include "../source/source.h"
#include "../filter/v_filter.h"
#include "../filter/movie_writer_filter.h"

class Render {

public:

    Render();

    void onSurfaceCreate(int w, int h);

    void onDraw();

    void onSurfaceDestroy();

    void setSource(Source *s);

    void setFilter(const std::shared_ptr<VFilter> &f);

    void start(JNIEnv *env, const char *video_path);

    void stop();

    void resume();

    void pause();

    virtual ~Render();

private:

    Source *source{nullptr};

    std::shared_ptr<VFilter> filter{nullptr};
    MovieWriterFilter *movieWriterFilter{nullptr};

    int width{0};
    int height{0};

};

#endif //SUPERPLAYER_RENDER_H
