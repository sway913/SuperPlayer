//
// Created by Lvlingling on 2020/8/2.
//

#ifndef SUPERPLAYER_PLAY_ENGINE_H
#define SUPERPLAYER_PLAY_ENGINE_H


#include "video_engine.h"

class PlayEngine : public VideoEngine {

public:

    void init(JNIEnv *env, const std::shared_ptr<GlView> &gl_view) override;

    void prepare(JNIEnv *env, const char *path) override;

    void resume() override;

    void pause() override;

    void stop() override;

    virtual ~PlayEngine();

private:

    Source *source{nullptr};
    IRender *render{nullptr};
};


#endif //SUPERPLAYER_PLAY_ENGINE_H
