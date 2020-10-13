//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_CAMERA_RENDER_H
#define SUPERPLAYER_CAMERA_RENDER_H


#include <memory>
#include <jni.h>
#include "../source/source.h"
#include "../filter/v_filter.h"
#include "../filter/movie_writer_filter.h"
#include "irender.h"

class CameraRender : public IRender {

public:

    CameraRender();

    void onSurfaceCreate(jobject surface, int w, int h) override;

    void onDraw() override;

    void onSurfaceDestroy() override;

    void setSource(Source *s) override;

    void setFilter(const std::shared_ptr<VFilter> &f) override;

    void start(JNIEnv *env, const char *video_path) override;

    void stop() override;

    void setState(bool isPause) override;

    void encodeFrame(uint8_t *frame) override;

    virtual ~CameraRender();

private:

    Source *source{nullptr};

    std::shared_ptr<VFilter> filter{nullptr};
    MovieWriterFilter *movieWriterFilter{nullptr};

    int width{0};
    int height{0};

};

#endif //SUPERPLAYER_CAMERA_RENDER_H
