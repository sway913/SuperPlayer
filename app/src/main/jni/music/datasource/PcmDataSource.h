//
// Created by Lvlingling on 2019-11-06.
//

#ifndef SUPERPLAYER_PCMDATASOURCE_H
#define SUPERPLAYER_PCMDATASOURCE_H


#include "IDataSource.h"
#include "oboe/Oboe.h"
#include <queue>
#include <future>
#include <iostream>
#include <fstream>
#include "../common/AudioData.h"
#include "../common/Resample.hpp"

using namespace oboe;

class PcmDataSource : public IDataSource {

private:

    std::future<void> sourceResult;
    std::future<void> filterResult;

    std::mutex mutex;
    std::condition_variable cond;

    STATUS status;

    std::ifstream *fileStream;

    char *readBuffer;
    int32_t readSize;
    int32_t totalSize;
    int32_t processCount = 0;
    int32_t readCount;

    LockFreeQueue<AudioData, MAX_QUEUE_PKTS> *sourceQueue{nullptr};
    LockFreeQueue<int16_t, MAX_QUEUE_NUM> *filterQueue{nullptr};

    Resample *resample;

    void loadData();

    void process();

public:
    PcmDataSource(AudioProperty outProperty, AudioProperty inProperty, const char *musicPath);

    void prepare() override;

    void start() override;

    void resume() override;

    void pause() override;

    void stop() override;

    bool getData(int16_t &value) override;

    void requestData() override;

    void seek(int64_t millis) override;

    bool isEmpty() override;

    bool isReady() override;

    int64_t getTotalMs() override;

    int64_t getCurrentMs() override;

    virtual ~PcmDataSource();


};


#endif //SUPERPLAYER_PCMDATASOURCE_H
