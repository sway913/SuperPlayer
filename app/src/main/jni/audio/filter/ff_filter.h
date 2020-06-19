//
// Created by ldr on 2019-12-16.
//

#ifndef SUPERPLAYER_FF_FILTER_H
#define SUPERPLAYER_FF_FILTER_H

#include <mutex>
#include <inttypes.h>
#include <SLES/OpenSLES.h>
#include "../../common/android_log.h"


extern "C" {
#include "libavutil/channel_layout.h"
#include "libavutil/mem.h"
#include "libavutil/opt.h"
#include "libavfilter/buffersrc.h"
#include "libavfilter/buffersink.h"
#include <libavfilter/avfilter.h>
}


#define EFFECT_PRESET_NONE          0
#define EFFECT_PRESET_CINEMA        1
#define EFFECT_PRESET_KTV           2
#define EFFECT_PRESET_STUDIO        3
#define EFFECT_PRESET_CONCERT       4
#define EFFECT_PRESET_CLASSIC_MAN   5
#define EFFECT_PRESET_RECORD_PLAYER 6
#define EFFECT_PRESET_CLASSIC_WOMAN 7
#define EFFECT_PRESET_VALLEY        8
#define EFFECT_PRESET_POP           9
#define EFFECT_PRESET_RB            10
#define EFFECT_PRESET_ROCK          11
#define EFFECT_PRESET_HIPHOP        12
#define EFFECT_PRESET_SPACIOUS      13
#define EFFECT_PRESET_ORIGINAL      14
#define EFFECT_PRESET_TEST 99

class FFFilter {

public:

    FFFilter(int sampleRate, int channelCount = 2);

    int32_t process(short *input, int len);

    void setFilter(int effect);

    void setVolume(double vol);

    ~FFFilter();

private:

    bool init();

    uint64_t processCount;
    int preset = 0;
    double volume = 2;
    bool isConfiguring;
    std::mutex configureMutex_;
    AVFilterGraph *filterGraph_{nullptr};
    AVFilterContext *avSrc_{nullptr};
    AVFilterContext *avSink_{nullptr};
    AVFrame *avFrame_{nullptr};

    int sample_rate{0};
    int channels{0};

protected:
    int initFilterGraph(AVFilterGraph **graph, AVFilterContext **src, AVFilterContext **sink);

    void printError(int err);

    bool reconfigure();
};


#endif //SUPERPLAYER_FF_FILTER_H
