//
// Created by Lvlingling on 2020/6/14.
//

#ifndef SUPERPLAYER_AUDIO_MERGER2_H
#define SUPERPLAYER_AUDIO_MERGER2_H

#include "../../common/merger_param.h"
#include "../codec/audio_encoder.h"
#include <cmath>
#include <utility>
#include <unistd.h>

class AudioMerger2 {

private:

    std::shared_ptr<MergerParam> sp_param{nullptr};

    AudioEncoder *audioEncoder{nullptr};
    double merger_bytes{0};
    double total_bytes{0};
    int buffer_size = 512;
    short *vocal_buffer{nullptr};
    short *acc_buffer{nullptr};
    short *mix_buffer{nullptr};

    void mergerFun();

public:

    void start(std::shared_ptr<MergerParam> sp_param);

    int getProgress();

};


#endif //SUPERPLAYER_AUDIO_MERGER2_H
