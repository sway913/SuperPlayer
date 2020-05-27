//
// Created by Lvlingling on 2019-11-06.
//

#include "PcmDataSource.h"
#include <utility>
#include "../common/AndroidLog.h"


PcmDataSource::PcmDataSource(AudioProperty outProperty, AudioProperty inProperty, const char *musicPath) : IDataSource(outProperty, inProperty) {

    sourceQueue = new LockFreeQueue<AudioData, MAX_QUEUE_PKTS>();
    filterQueue = new LockFreeQueue<int16_t, MAX_QUEUE_NUM>();

    fileStream = new std::ifstream(musicPath, std::ios::binary);
    fileStream->seekg(0, std::ios::beg);
    fileStream->seekg(0, std::ios::end);
    totalSize = (int) fileStream->tellg();
    fileStream->seekg(0, std::ios::beg);
    readSize = 0;
    readBuffer = new char[READ_SIZE];
    readCount = 0;
    resample = new Resample(inProperty, outProperty);
}

void PcmDataSource::prepare() {
    status = INIT;
    processCount = 0;
    filterResult = std::async(std::launch::async, &PcmDataSource::process, this);
    sourceResult = std::async(std::launch::async, &PcmDataSource::loadData, this);
}

void PcmDataSource::loadData() {
    if (status != INIT) {
        return;
    }
    while (status != STOP) {

        if (status == PAUSE) {
            continue;
        }
        if (readSize >= totalSize) {
            continue;
        }
        if (sourceQueue->size() > CACHE_FRAME_NUM) {
            continue;
        }
        memset(readBuffer, 0, READ_SIZE);
        int32_t remainSize = totalSize - readSize;
        int32_t frameSize = 0;
        if (remainSize >= READ_SIZE) {
            frameSize = READ_SIZE;
        } else {
            frameSize = remainSize;
        }
        fileStream->read(readBuffer, frameSize);
        frameSize = static_cast<int32_t>(fileStream->gcount());
        readSize += frameSize;
        //vocalFileStream->seekg(readSize, std::ios::beg);

        int in_sample = static_cast<int>((float) frameSize / ((float) inProperty.channelCount * sizeof(int16_t)));
        int out_sample = static_cast<int>((float) in_sample * ((float) outProperty.sampleRate / (float) inProperty.sampleRate) * 1.2);
        int out_size = static_cast<int>(out_sample * outProperty.channelCount * sizeof(int16_t));
        uint8_t *out = new uint8_t[out_size];
        int len = resample->resample(out, out_sample, reinterpret_cast<const uint8_t *>(readBuffer), in_sample);
        if (len > 0) {
            int size = len * outProperty.channelCount * sizeof(int16_t);
            sourceQueue->push(AudioData(reinterpret_cast<char *>(out), size));
            if (++readCount == CACHE_FRAME_NUM) {
                cond.notify_all();
                LOGI("pcm notify");
            }
        }
        delete[] out;
    }
}


void PcmDataSource::process() {
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

        int maxSize = outProperty.channelCount * outProperty.sampleRate * 80 / 1000;
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
                if (processCount == 1) {
                    LOGI("pcm filter ready");
                }
            } else {
                //LOGI("pcm soure pop fail");
            }

        }
    }
}

bool PcmDataSource::getData(int16_t &value) {
    return filterQueue->pop(value);
}

void PcmDataSource::requestData() {
    cond.notify_all();
}


void PcmDataSource::start() {

}

void PcmDataSource::resume() {

}

void PcmDataSource::pause() {

}

void PcmDataSource::stop() {
    status = STOP;
    fileStream->close();
    cond.notify_all();
    std::future_status futureStatus;
    do {
        futureStatus = sourceResult.wait_for(std::chrono::milliseconds(100));
    } while (futureStatus != std::future_status::ready);
    do {
        futureStatus = filterResult.wait_for(std::chrono::milliseconds(100));
    } while (futureStatus != std::future_status::ready);
}

void PcmDataSource::seek(int64_t millis) {
    while (sourceQueue->size() > 0) {
        AudioData audioData;
        sourceQueue->pop(audioData);
    }
    filterQueue->reset();
    sourceQueue->reset();
    int32_t readByte = inProperty.channelCount * inProperty.sampleRate * sizeof(int16_t) / 1000 * millis;
    fileStream->seekg(readByte);
    readSize = readByte;
    readCount = 0;
    processCount = 0;
}

bool PcmDataSource::isEmpty() {
    return readSize >= totalSize && sourceQueue->size() == 0 && filterQueue->size() == 0;
}

bool PcmDataSource::isReady() {
    return processCount > 0;
}

int64_t PcmDataSource::getTotalMs() {
    return 0;
}

int64_t PcmDataSource::getCurrentMs() {
    return 0;
}

PcmDataSource::~PcmDataSource() {
    delete sourceQueue;
    sourceQueue = nullptr;
    delete filterQueue;
    filterQueue = nullptr;
    delete fileStream;
    fileStream = nullptr;
    delete readBuffer;
    readBuffer = nullptr;
    delete resample;
    resample = nullptr;
}
