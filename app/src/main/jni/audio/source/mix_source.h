//
// Created by ldr on 2020/6/5.
//

#ifndef SUPERPLAYER_MIX_SOURCE_H
#define SUPERPLAYER_MIX_SOURCE_H

#include <future>
#include <vector>
#include "isource.h"
#include "../../common/lock_free_queue.h"
#include "../../common/common_tools.h"
#include "../filter/filter_package.h"

using namespace std;

class MixSource {

public:

    MixSource(int sample_rate);

    virtual ~MixSource();

    void addSource(ISource *s);

    void start();

    void pause();

    void resume();

    void stop();

    void seek(int64_t ms);

    long getCurrentMs();

    void getMixData(short *out, int numFrames, short *input, int frameRead);

private:

    future<void> threadResult{};
    vector<ISource *> source;
    mutex _mutex{};
    condition_variable cond{};
    LockFreeQueue<short, kMixDataSize> *data_queue{nullptr};

    void mixData();

    bool isExit{true};
    bool isPause{false};

    short *audioBuffer{nullptr};
    short *mixBuffer{nullptr};
    int bufferSize = 1024;
    long current_ms{0};
    int sample_rate{0};

};


#endif //SUPERPLAYER_MIX_SOURCE_H
