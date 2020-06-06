//
// Created by ldr on 2020/6/5.
//

#include "mix_source.h"

MixSource::MixSource() {
    data_queue = new LockFreeQueue<short, DATA_QUEUE_SIZE>();
    audioBuffer = new short[bufferSize];
    mixBuffer = new short[bufferSize];
}

void MixSource::start() {
    threadResult = async(launch::async, &MixSource::mixData, this);
}

void MixSource::resume() {
    isPause = false;
    cond.notify_all();
}

void MixSource::pause() {
    isPause = true;
}

void MixSource::stop() {
    isExit = true;
    cond.notify_all();
    JOIN(threadResult);
    source.clear();
}

void MixSource::reset() {
    data_queue->reset();
}


void MixSource::addSource(ISource *s) {
    if (s) {
        source.push_back(s);
    }
}

void MixSource::getData(short *audioData, int numFrames) {
    if (isPause) {
        memset(audioData, 0, numFrames * 2);
        return;
    }
    for (int i = 0; i < numFrames * 2; ++i) {
        if (!data_queue->pop(audioData[i])) {
            audioData[i] = 0;
        }
    }
    cond.notify_all();
}


void MixSource::mixData() {
    while (!isExit) {
        unique_lock<mutex> lock(_mutex);
        cond.wait(lock, [this] {
            if (isExit) {
                return true;
            } else if (isPause) {
                return false;
            } else {
                return data_queue->size() < DATA_QUEUE_SIZE / 2;
            }
        });

        if (isExit) {
            break;
        }

        if (!source.empty()) {
            ISource *pSource = source[0];
            if (pSource->isReady()) {
                for (int i = 0; i < bufferSize; ++i) {
                    if (!pSource->getData(audioBuffer[i])) {
                        audioBuffer[i] = 0;
                    }
                }
            } else {
                memset(audioBuffer, 0, bufferSize * sizeof(short));
            }
            pSource->requestData();
        } else {
            memset(audioBuffer, 0, bufferSize * sizeof(short));
        }

        if (source.size() > 1) {
            ISource *pSource = source[1];
            if (pSource->isReady()) {
                for (int i = 0; i < bufferSize; ++i) {
                    if (!pSource->getData(mixBuffer[i])) {
                        mixBuffer[i] = 0;
                    }
                }
            } else {
                memset(mixBuffer, 0, bufferSize * sizeof(short));
            }
            pSource->requestData();
        } else {
            memset(mixBuffer, 0, bufferSize * sizeof(short));
        }

        for (int i = 0; i < bufferSize; ++i) {
            short value = mixAudioData(audioBuffer[i], mixBuffer[i]);
            data_queue->push(value);
        }
    }
}

short MixSource::mixAudioData(short a, short b) {
    int tmp = a < 0 && b < 0 ? ((int) a + (int) b) - (((int) a * (int) b) / INT16_MIN) : (a > 0 && b > 0 ? ((int) a + (int) b) - (((int) a * (int) b) / INT16_MAX) : a + b);
    return static_cast<int16_t>(tmp > INT16_MAX ? INT16_MAX : (tmp < INT16_MIN ? INT16_MIN : tmp));
}

MixSource::~MixSource() {
    DELETEOBJ(data_queue)
    DELETEARR(audioBuffer)
    DELETEARR(mixBuffer)
}
