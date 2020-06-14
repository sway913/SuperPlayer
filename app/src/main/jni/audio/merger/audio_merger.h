//
// Created by Lvlingling on 2020/6/12.
//

#ifndef SUPERPLAYER_AUDIO_MERGER_H
#define SUPERPLAYER_AUDIO_MERGER_H


#include "../../common/merger_param.h"
#include "../codec/audio_encoder.h"
#include "../codec/audio_decoder.h"
#include "../../common/circle_buffer.h"
#include <cmath>
#include <utility>
#include <unistd.h>

#define MERGER_BUFFER_CAPBILATY 96000

class AudioMerger {

private:

    std::shared_ptr<MergerParam> sp_param{nullptr};

    AudioEncoder *audioEncoder{nullptr};
    AudioDecoder *accDecoder{nullptr};
    AudioDecoder *vocalDecoder{nullptr};
    CircleBuffer<short> *acc_buffer{nullptr};
    CircleBuffer<short> *vocal_buffer{nullptr};
    int acc_key{0};
    int vocal_key{0};
    double merger_bytes{0};
    double total_bytes{0};

    void mergerFun();

public:

    void start(std::shared_ptr<MergerParam> sp_param);

    int getProgress();

};


#endif //SUPERPLAYER_AUDIO_MERGER_H
