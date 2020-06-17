//
// Created by ldr on 2020/6/5.
//

#include "mix_source.h"

MixSource::MixSource(int sample) {
    data_queue = new LockFreeQueue<short, kDataQueueSize>();
    audioBuffer = new short[bufferSize];
    mixBuffer = new short[bufferSize];
    this->sample_rate = sample;
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

void MixSource::seek(int64_t ms) {
    data_queue->reset();
    current_ms = ms;
}


void MixSource::addSource(ISource *s) {
    if (s) {
        source.push_back(s);
    }
}

void MixSource::getMixData(short *out, int numFrames, short *input, int frameRead) {
    if (isPause) {
        memset(out, 0, numFrames * 2);
        return;
    }
    for (int i = 0; i < numFrames; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (!data_queue->pop(out[2 * i + j])) {
                out[2 * i + j] = 0;
            }
            if (input && i < frameRead) {
                out[2 * i + j] = mixAudioSample(out[2 * i + j], input[i]);
            }
        }
    }
    current_ms += (long)((float)numFrames * 1000 / sample_rate);
    cond.notify_all();
}

long MixSource::getCurrentMs() {
    return current_ms;
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
                return data_queue->size() < kDataQueueSize / 2;
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
            short value = mixAudioSample(audioBuffer[i], mixBuffer[i]);
            data_queue->push(value);
        }
    }
}

MixSource::~MixSource() {
    DELETEOBJ(data_queue)
    DELETEARR(audioBuffer)
    DELETEARR(mixBuffer)
}
