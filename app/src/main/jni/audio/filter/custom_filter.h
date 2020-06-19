//
// Created by ldr on 2020/6/19.
//

#ifndef SUPERPLAYER_CUSTOM_FILTER_H
#define SUPERPLAYER_CUSTOM_FILTER_H

#include "ifilter.h"
#include "../../common/common_tools.h"


#ifdef NDEBUG
#undef NDEBUG
#endif

extern "C" {
#include "../../thirdpart/sox/src/sox.h"
}

#include <cmath>
#include <cassert>
#include <cstdlib>

class CustomFilter : public IFilter {

public:

    void init(int sampleRate, int channelCount) override final;

    int process(short *data, int len) override final;

    void setEffect(float *arr);

    void destroy() override final;

    static short *input_data;
    static int input_size;
    static short *out_data;

private:

    void initChain(float *arr);

    void destroyChain();

    sox_sample_t sample_rate;
    int channels;
    sox_signalinfo_t signal;
    sox_signalinfo_t mono_signal;
    sox_encodinginfo_t encoding;

    sox_effects_chain_t *effect_chain{nullptr};

    bool inited{false};

};


#endif //SUPERPLAYER_CUSTOM_FILTER_H
