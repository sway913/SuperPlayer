//
// Created by ldr on 2020/6/5.
//

#ifndef SUPERPLAYER_MIX_SOURCE_H
#define SUPERPLAYER_MIX_SOURCE_H

#include <future>
#include <vector>
#include "isource.h"
#include "../../common/lock_free_queue.h"

using namespace std;

class MixSource {

public:

    MixSource();

    virtual ~MixSource();

    void addSource(ISource *s);

    void start();

    void pause();

    void resume();

    void stop();

    void reset();

    void getData(short *audioData, int numFrames);

private:

    future<void> threadResult{};
    vector<ISource *> source;
    mutex _mutex{};
    condition_variable cond{};
    LockFreeQueue<short, DATA_QUEUE_SIZE> *data_queue{nullptr};

    void mixData();

    static inline short mixAudioData(short a, short b);

    bool isExit{false};
    bool isPause{false};

    short *audioBuffer{nullptr};
    short *mixBuffer{nullptr};
    int bufferSize = 512;

};


#endif //SUPERPLAYER_MIX_SOURCE_H
