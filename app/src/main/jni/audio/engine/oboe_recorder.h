//
// Created by ldr on 2020/6/6.
//

#ifndef SUPERPLAYER_OBOE_RECORDER_H
#define SUPERPLAYER_OBOE_RECORDER_H

#include <oboe/Oboe.h>
#include "../../common/constexpr.h"

using namespace oboe;

enum class RecordState {
    ready = 0,
    preparing = 1,
    fail = 2,
    success = 3
};

class OboeRecorder {

public:

    int start(int sampleRate);

    void stop();

    int read(short *buffer, int numFrames);

    RecordState getState(int16_t *data, int32_t numFrames);

private:

    int32_t count_callback_to_drain = KRecorderCallbackCount;
    int32_t count_callback_to_discard = KRecorderDrainCount;

    ManagedStream inputStream{nullptr};
};


#endif //SUPERPLAYER_OBOE_RECORDER_H
