//
// Created by ldr on 2019-11-10.
//

#ifndef SUPERPLAYER_STREAMDATASOURCE_H
#define SUPERPLAYER_STREAMDATASOURCE_H

#include "IDataSource.h"
#include "oboe/Oboe.h"
#include <queue>
#include <future>
#include <iostream>
#include <fstream>
#include "../common/AudioData.h"
#include "../decode/FFDecoder.h"
#include "../../soundtouch/include/SoundTouch.h"


class StreamDataSource : public IDataSource {

    std::future<void> sourceResult;
    std::future<void> filterResult;

    std::mutex mutex;
    std::condition_variable cond;

    STATUS status;

    int32_t processCount = 0;
    int readCount = 0;

    LockFreeQueue<AudioData, MAX_QUEUE_PKTS> *sourceQueue{nullptr};
    LockFreeQueue<int16_t, MAX_QUEUE_NUM> *filterQueue{nullptr};

    FFDecoder *ffDecoder;

    int64_t currentMs = 0;

    void loadData();

    void process();

public:

    StreamDataSource(AudioProperty outAudioProperty, const char *musicPath);

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

    virtual ~StreamDataSource();

};


#endif //SUPERPLAYER_STREAMDATASOURCE_H
