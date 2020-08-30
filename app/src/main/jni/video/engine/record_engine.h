//
// Created by Lvlingling on 2020/8/2.
//

#ifndef SUPERPLAYER_RECORD_ENGINE_H
#define SUPERPLAYER_RECORD_ENGINE_H


#include "video_engine.h"
#include "../render/camera_render.h"
#include "../filter/combine_filter.h"


class RecordEngine : public VideoEngine {

public:

    void init(JNIEnv *env, const std::shared_ptr<GlView> &gl_view) override;

    void stop() override;

    void start() override;

    void switchCamera() override;

    void setEffect(std::shared_ptr<VideoEffect> &effect) override;

    void prepare(JNIEnv *env, const char *path) override;

    void resume() override;

    void pause() override;

    virtual ~RecordEngine();

private:

    Source *source{nullptr};
    CameraRender *render{nullptr};
    std::shared_ptr<CombineFilter> combine_filter{nullptr};

};


#endif //SUPERPLAYER_RECORD_ENGINE_H
