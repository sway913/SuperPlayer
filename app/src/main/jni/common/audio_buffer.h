//
// Created by ldr on 2020/6/5.
//

#ifndef SUPERPLAYER_AUDIO_BUFFER_H
#define SUPERPLAYER_AUDIO_BUFFER_H

#include "lock_free_queue.h"
#include "constexpr.h"

using namespace std;

class AudioFrame {

public:
    int len{0};
    char *audio_data{nullptr};

public:
    AudioFrame(char *data, int len) {
        this->len = len;
        if (len > 0) {
            audio_data = new char[len];
            memcpy(audio_data, data, len);
        }
    }

    virtual ~AudioFrame() {
        DELETEARR(audio_data)
    }
};

class AudioBuffer {

public:

    LockFreeQueue<AudioFrame *, KDecodeBufferSize> *audio_queue{nullptr};

public:

    AudioBuffer() {
        audio_queue = new LockFreeQueue<AudioFrame *, KDecodeBufferSize>();
    }

    void push(char *data, int len) {
        audio_queue->push(new AudioFrame(data, len));
    }

    bool isEmpty() {
        return audio_queue->size() == 0;
    }

    AudioFrame *pop() {
        AudioFrame *frame = nullptr;
        audio_queue->pop(frame);
        return frame;
    }

    virtual ~AudioBuffer() {
        while (!isEmpty()) {
            AudioFrame *frame = nullptr;
            audio_queue->pop(frame);
            DELETEOBJ(frame)
        }
    }
};


#endif //SUPERPLAYER_AUDIO_BUFFER_H
