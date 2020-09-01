//
// Created by Lvlingling on 2020/6/14.
//

#ifndef SUPERPLAYER_AUDIO_MERGER2_H
#define SUPERPLAYER_AUDIO_MERGER2_H

#include "../../common/merger_param.h"
#include "../codec/audio_encoder.h"
#include "../filter/filter_package.h"
#include "../codec/i_video_merger.h"
#include <cmath>
#include <utility>
#include <unistd.h>
#include <jni.h>

class AudioMerger2 {

private:

    std::shared_ptr<MergerParam> sp_param{nullptr};

    IAudioEncoder *audioEncoder{nullptr};
    IVideoMerger *videoMerger{nullptr};
    int64_t merger_bytes{0};
    int64_t total_bytes{2 << 16};
    int buffer_size = 1024;
    short *vocal_buffer{nullptr};
    short *acc_buffer{nullptr};

    FilterPackage *vocal_filter{nullptr};
    FilterPackage *acc_filter{nullptr};

    void mergerFun();

    bool finish{false};

public:

    void start(JNIEnv *env, std::shared_ptr<MergerParam> sp_param);

    int getProgress();

};


#endif //SUPERPLAYER_AUDIO_MERGER2_H
