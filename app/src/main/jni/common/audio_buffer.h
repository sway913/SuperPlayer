//
// Created by ldr on 2020/6/5.
//

#ifndef SUPERPLAYER_AUDIO_BUFFER_H
#define SUPERPLAYER_AUDIO_BUFFER_H

#include <queue>
#include "lock_free_queue.h"

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

    queue<AudioFrame *> audio_queue;
    mutex _mutex;

public:

    void push(char *data, int len) {
        lock_guard<mutex> lock(_mutex);
        audio_queue.push(new AudioFrame(data, len));
    }

    bool isEmpty() {
        lock_guard<mutex> lock(_mutex);
        return audio_queue.empty();
    }

    AudioFrame *pop() {
        lock_guard<mutex> lock(_mutex);
        AudioFrame *frame;
        if(audio_queue.empty()){
            frame = nullptr;
        } else {
            frame = audio_queue.front();
            audio_queue.pop();
        }
        return frame;
    }

    virtual ~AudioBuffer() {
        lock_guard<mutex> lock(_mutex);
        while (!audio_queue.empty()) {
            AudioFrame *frame = audio_queue.front();
            audio_queue.pop();
            DELETEOBJ(frame)
        }
    }
};


#endif //SUPERPLAYER_AUDIO_BUFFER_H
