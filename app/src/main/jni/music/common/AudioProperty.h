//
// Created by ldr on 2019-11-21.
//


#ifndef SUPERPLAYER_AUDIOPROPERTY_H
#define SUPERPLAYER_AUDIOPROPERTY_H

extern "C" {
#include <libavutil/samplefmt.h>
};

struct AudioProperty {
    enum AVSampleFormat sampleFormat = AV_SAMPLE_FMT_S16;
    int sampleRate = 44100;
    int channelCount = 2;
};

#endif //SUPERPLAYER_AUDIOPROPERTY_H