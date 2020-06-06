//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_PLAYER_ENGINE_H
#define SUPERPLAYER_PLAYER_ENGINE_H


#include "audio_engine.h"

class PlayerEngine : public AudioEngine {

public:

    PlayerEngine(int sampleRate);

    void prepare(SourceFactory *factory) override final;

    void start() override final;

    DataCallbackResult onAudioReady(AudioStream *stream, void *data, int32_t numFrames) override final;


private:


};


#endif //SUPERPLAYER_PLAYER_ENGINE_H
