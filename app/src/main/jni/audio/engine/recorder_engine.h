//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_RECORDER_ENGINE_H
#define SUPERPLAYER_RECORDER_ENGINE_H


#include "audio_engine.h"

class RecorderEngine: public AudioEngine {

public:
    RecorderEngine(int sampleRate);

};


#endif //SUPERPLAYER_RECORDER_ENGINE_H
