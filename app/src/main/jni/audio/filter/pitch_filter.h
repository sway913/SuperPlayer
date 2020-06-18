//
// Created by Lvlingling on 2020/6/17.
//

#ifndef SUPERPLAYER_PITCH_FILTER_H
#define SUPERPLAYER_PITCH_FILTER_H

#include "ifilter.h"
#include "../../thirdpart/soundtouch/include/SoundTouch.h"

using namespace soundtouch;
class PitchFilter : public IFilter {

private:

    SoundTouch *soundTouch{nullptr};
    int channels{2};
    float pitch_shift{0};

public:

    void init(int sampleRate, int channel, int param) override final;

    int process(short *data, int len) override final;

    void destroy() override final;

    void setPitch(float pitch);

    FilterType getType() override final;

};


#endif //SUPERPLAYER_PITCH_FILTER_H
