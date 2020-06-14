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

    void reset();

    long getCurrentMs();

    void getMixData(short *out, int numFrames, short *input, int frameRead);

private:

    future<void> threadResult{};
    vector<ISource *> source;
    mutex _mutex{};
    condition_variable cond{};
    LockFreeQueue<short, kDataQueueSize> *data_queue{nullptr};

    void mixData();

    bool isExit{false};
    bool isPause{false};

    short *audioBuffer{nullptr};
    short *mixBuffer{nullptr};
    int bufferSize = 512;
    long current_ms{0};
    int sample_rate{0};

};


#endif //SUPERPLAYER_MIX_SOURCE_H
