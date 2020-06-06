//
// Created by ldr on 2020/6/6.
//

#ifndef SUPERPLAYER_TEST_OBOE_H
#define SUPERPLAYER_TEST_OBOE_H

#include <oboe/Oboe.h>
#include <fstream>

using namespace oboe;

class TestOboe : public AudioStreamCallback {

public:

    void start();

    void stop();

    DataCallbackResult onAudioReady(AudioStream *stream, void *data, int32_t numFrames) override;

private:

    std::ifstream fileStream{};
    ManagedStream outStream{nullptr};

};


#endif //SUPERPLAYER_TEST_OBOE_H
