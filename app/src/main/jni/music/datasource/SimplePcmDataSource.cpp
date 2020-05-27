//
// Created by ldr on 2019-12-06.
//

#include "SimplePcmDataSource.h"
#include <utility>
#include "../common/AndroidLog.h"


SimplePcmDataSource::SimplePcmDataSource(AudioProperty outProperty, AudioProperty inProperty, const char *musicPath) : IDataSource(outProperty, inProperty) {

    pcmDataQueue = new LockFreeQueue<int16_t, MAX_QUEUE_NUM>();

    fileStream = new std::ifstream(musicPath, std::ios::binary);
    fileStream->seekg(0, std::ios::beg);
    fileStream->seekg(0, std::ios::end);
    totalSize = (int) fileStream->tellg();
    fileStream->seekg(0, std::ios::beg);
    readSize = 0;
    readBuffer = new char[READ_SIZE];

    this->outProperty = outProperty;
    this->inProperty = inProperty;
    resample = new Resample(inProperty, outProperty);
}

void SimplePcmDataSource::prepare() {
    status = INIT;
    readCount = 0;
    dataResult = std::async(std::launch::async, &SimplePcmDataSource::loadData, this);
}

void SimplePcmDataSource::loadData() {
    if (status != INIT) {
        return;
    }
    while (status != STOP) {

        if (readCount > 0) {
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait(lock);
        }

        if (status == PAUSE) {
            continue;
        }
        if (readSize >= totalSize) {
            continue;
        }

        int cacheSize = outProperty.channelCount * outProperty.sampleRate * 80 / 1000;
        int bufferSize = pcmDataQueue->size();

        if (bufferSize >= cacheSize) {
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

        processData(readBuffer,frameSize);

        int in_sample = static_cast<int>((float) frameSize / ((float) inProperty.channelCount * sizeof(int16_t)));
        int out_sample = static_cast<int>((float) in_sample * ((float) outProperty.sampleRate / (float) inProperty.sampleRate) * 1.1);
        int out_size = static_cast<int>(out_sample * outProperty.channelCount * sizeof(int16_t));
        uint8_t *out = new uint8_t[out_size];
        int len = resample->resample(out, out_sample, reinterpret_cast<const uint8_t *>(readBuffer), in_sample);
        if (len > 0) {
            int32_t size = len * outProperty.channelCount;
            auto numSamples = size;
            int16_t *outBuffer = (int16_t *) (out);
            for (int j = 0; j < numSamples; j++) {
                if (!pcmDataQueue->push(outBuffer[j])) {
                    LOGI("push fail");
                }
            }
            readCount++;
            if (readCount == 1) {
                LOGI("pcm filter ready");
            }
        } else {
            LOGI("resample error");
        }
        delete[] out;
    }
}

bool SimplePcmDataSource::getData(int16_t &value) {
    return pcmDataQueue->pop(value);
}

void SimplePcmDataSource::requestData() {
    cond.notify_all();
}

void SimplePcmDataSource::start() {

}

void SimplePcmDataSource::resume() {

}

void SimplePcmDataSource::pause() {

}

void SimplePcmDataSource::stop() {
    status = STOP;
    fileStream->close();
    cond.notify_all();
    std::future_status futureStatus;
    do {
        futureStatus = dataResult.wait_for(std::chrono::milliseconds(100));
    } while (futureStatus != std::future_status::ready);
}

void SimplePcmDataSource::seek(int64_t millis) {
    pcmDataQueue->reset();
    int32_t readByte = inProperty.channelCount * inProperty.sampleRate * sizeof(int16_t) / 1000 * millis;
    fileStream->seekg(readByte);
    readSize = readByte;
    readCount = 0;
    cond.notify_all();
}

bool SimplePcmDataSource::isEmpty() {
    return pcmDataQueue->size() == 0;
}

bool SimplePcmDataSource::isReady() {
    return readCount > 0;
}

int64_t SimplePcmDataSource::getTotalMs() {
    return 0;
}

int64_t SimplePcmDataSource::getCurrentMs() {
    return 0;
}

SimplePcmDataSource::~SimplePcmDataSource() {
    delete pcmDataQueue;
    pcmDataQueue = nullptr;
    delete fileStream;
    fileStream = nullptr;
    delete readBuffer;
    readBuffer = nullptr;
    delete resample;
    resample = nullptr;
}
