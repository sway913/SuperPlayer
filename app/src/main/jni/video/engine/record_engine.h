//
// Created by Lvlingling on 2020/8/2.
//

#ifndef SUPERPLAYER_RECORD_ENGINE_H
#define SUPERPLAYER_RECORD_ENGINE_H


#include "video_engine.h"
#include "../render/render.h"
#include "../filter/combine_filter.h"


class RecordEngine : public VideoEngine {

public:

    RecordEngine(JNIEnv *env);

    void start() override;

    void stop() override;

    void initGlView() override;

    void switchCamera() override;

    void setEffect(std::shared_ptr<VideoEffect> &effect) override;

    virtual ~RecordEngine();

private:

    Source *source{nullptr};
    Render *render{nullptr};
    std::shared_ptr<CombineFilter> combine_filter{nullptr};

};


#endif //SUPERPLAYER_RECORD_ENGINE_H
