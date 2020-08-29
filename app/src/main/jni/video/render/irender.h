//
// Created by Lvlingling on 2020/8/12.
//

#ifndef SUPERPLAYER_IRENDER_H
#define SUPERPLAYER_IRENDER_H

#include <memory>
#include <jni.h>
#include "../source/source.h"
#include "../filter/v_filter.h"
#include "../filter/movie_writer_filter.h"

class IRender {

public:

    virtual void onSurfaceCreate(jobject surface, int w, int h) {};

    virtual void onDraw() {};

    virtual void onSurfaceDestroy() {};

    virtual void setSource(Source *s) {};

    virtual void setFilter(const std::shared_ptr<VFilter> &f) {};

    virtual void start(JNIEnv *env, const char *video_path) {};

    virtual void stop() {};

    virtual void setState(bool isPause) {};

    virtual ~IRender() {};

};


#endif //SUPERPLAYER_IRENDER_H
