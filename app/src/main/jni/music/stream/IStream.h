//
// Created by ldr on 2019-11-18.
//

#ifndef SUPERPLAYER_ISTREAM_H
#define SUPERPLAYER_ISTREAM_H


#include <iostream>
#include "../common/AudioProperty.h"
#include "../common/AndroidLog.h"
#include "../common/Observer.h"

using namespace std;

class IStream {

protected:
    Observer *observer{nullptr};

public:

    IStream() {}

    virtual ~IStream() {
    }

    virtual void prepare(const char *musicPath, const char *guidePath, const char *vocalPath) = 0;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void resume() = 0;

    virtual void pause() = 0;

    virtual void setEcho(bool isEcho) = 0;

    virtual void seek(int64_t millis) = 0;

    virtual void setVolume(float volume,int track) = 0;

    virtual void setFilter(int type) = 0;

    virtual void setPitch(int pitch) = 0;

    virtual void setObserver(Observer *observer) {
        this->observer = observer;
    }

    virtual bool onAudioDataReady(int16_t *data, int32_t numFrames) = 0;

    virtual void onDataReady() = 0;

    virtual int64_t getTotalMs() = 0;

    virtual int64_t getCurrentMs() = 0;

};

#endif

