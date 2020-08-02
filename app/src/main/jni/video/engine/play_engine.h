//
// Created by Lvlingling on 2020/8/2.
//

#ifndef SUPERPLAYER_PLAY_ENGINE_H
#define SUPERPLAYER_PLAY_ENGINE_H


#include "video_engine.h"

class PlayEngine : public VideoEngine {

public:

    PlayEngine(JNIEnv *env);

    void start() override;

    void stop() override;

    void initGlView() override;

};


#endif //SUPERPLAYER_PLAY_ENGINE_H
