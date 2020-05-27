//
// Created by ldr on 2019-12-06.
//

#ifndef SUPERPLAYER_SIMPLEPCMDATASOURCE_H
#define SUPERPLAYER_SIMPLEPCMDATASOURCE_H


#include "IDataSource.h"
#include "oboe/Oboe.h"
#include <queue>
#include <future>
#include <iostream>
#include <fstream>
#include "../common/AudioData.h"
#include "../common/Resample.hpp"

using namespace oboe;

class SimplePcmDataSource : public IDataSource {

private:

    std::future<void> dataResult;

    std::mutex mutex;
    std::condition_variable cond;

    STATUS status;

    std::ifstream *fileStream;

    char *readBuffer;
    int32_t readSize;
    int32_t totalSize;
    int32_t readCount = 0;

    LockFreeQueue<int16_t, MAX_QUEUE_NUM> *pcmDataQueue{nullptr};

    Resample *resample;

    void loadData();


public:

    SimplePcmDataSource(AudioProperty outProperty,AudioProperty inProperty,const char *musicPath);

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

    virtual ~SimplePcmDataSource();

};


#endif //SUPERPLAYER_SIMPLEPCMDATASOURCE_H
