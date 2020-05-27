//
// Created by ldr on 2019-11-10.
//

#include "StreamDataSource.h"

StreamDataSource::StreamDataSource(AudioProperty outProperty, const char *musicPath) : IDataSource(outProperty, AudioProperty()) {

    sourceQueue = new LockFreeQueue<AudioData, MAX_QUEUE_PKTS>();
    filterQueue = new LockFreeQueue<int16_t, MAX_QUEUE_NUM>();

    ffDecoder = new FFDecoder();
    if (ffDecoder->prepare(musicPath, outProperty)) {
        // LOGI("decoder prepare success");
        //LOGI("music path %s:", musicPath);
    } else {
        //LOGE("decoder prepare fail");
    }
}

void StreamDataSource::prepare() {
    status = INIT;
    processCount = 0;
    sourceResult = std::async(std::launch::async, &StreamDataSource::loadData, this);
    filterResult = std::async(std::launch::async, &StreamDataSource::process, this);
}

void StreamDataSource::loadData() {
    if (status != INIT) {
        return;
    }
    while (status != STOP) {

        if (status == PAUSE) {
            continue;
        }
        if (sourceQueue->size() > CACHE_FRAME_NUM) {
            continue;
        }

        AudioData audioData = ffDecoder->decodeFrame();
        if (audioData.size <= 0 || audioData.data == nullptr) {
            continue;
        }
        sourceQueue->push(audioData);

        if (++readCount == CACHE_FRAME_NUM) {
            cond.notify_all();
            // LOGI("decode ready");
        }
    }
}


void StreamDataSource::process() {
    if (status != INIT) {
        return;
    }
    while (status != STOP) {

        if (status == PAUSE) {
            continue;
        }

        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock);

        int count = 1;
        if (processCount == 0) {
            count = CACHE_FRAME_NUM;
        }

        int maxSize = outProperty.channelCount * outProperty.sampleRate * 40 / 10000;
        int remainSize = filterQueue->size();

        if (remainSize >= maxSize) {
            continue;
        }

        for (int i = 0; i < count; ++i) {
            AudioData audioData;
            if (sourceQueue->pop(audioData)) {

                processData(audioData.data, audioData.size);

                auto numSamples = static_cast<int32_t>(audioData.size / sizeof(int16_t));
                int16_t *outBuffer = (int16_t *) (audioData.data);
                for (int j = 0; j < numSamples; j++) {
                    filterQueue->push(outBuffer[j]);
                }
                processCount++;
            }
        }
    }
}

bool StreamDataSource::getData(int16_t &value) {
    return filterQueue->pop(value);
}

void StreamDataSource::requestData() {
    cond.notify_all();
}


void StreamDataSource::start() {

}

void StreamDataSource::resume() {

}

void StreamDataSource::pause() {

}

void StreamDataSource::stop() {
    status = STOP;
    cond.notify_all();
    ffDecoder->close();
    std::future_status futureStatus;
    do {
        futureStatus = sourceResult.wait_for(std::chrono::milliseconds(100));
    } while (futureStatus != std::future_status::ready);
    do {
        futureStatus = filterResult.wait_for(std::chrono::milliseconds(100));
    } while (futureStatus != std::future_status::ready);
}

void StreamDataSource::seek(int64_t millis) {
    readCount = 0;
    processCount = 0;
    status = PAUSE;
    while (sourceQueue->size() > 0) {
        AudioData audioData = AudioData();
        sourceQueue->pop(audioData);
    }
    sourceQueue->reset();
    filterQueue->reset();
    ffDecoder->seek(millis);
    status = PLAYING;
}

bool StreamDataSource::isEmpty() {
    return sourceQueue->size() == 0 && filterQueue->size() == 0;
}

bool StreamDataSource::isReady() {
    return processCount > 0;
}

int64_t StreamDataSource::getTotalMs() {
    return ffDecoder->getTotalMs();
}

int64_t StreamDataSource::getCurrentMs() {
    return currentMs;
}

StreamDataSource::~StreamDataSource() {
    delete sourceQueue;
    sourceQueue = nullptr;
    delete filterQueue;
    filterQueue = nullptr;
    delete ffDecoder;
    ffDecoder = nullptr;
}