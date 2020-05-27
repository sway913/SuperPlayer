//
// Created by ldr on 2019-11-08.
//

#ifndef SUPERPLAYER_BASEENGINE_H
#define SUPERPLAYER_BASEENGINE_H


#include "oboe/Oboe.h"
#include "../common/Common.h"
#include "../stream/IStream.h"
#include "../common/Observer.h"

using namespace oboe;

class BaseEngine : public AudioStreamCallback {

private:
    bool isRecordMode;

    bool initOboe(int sample);

protected:

    STATUS status = ERROR;
    AudioStream *outputStream{nullptr};
    AudioStream *inputStream{nullptr};
    IStream *sourceStream{nullptr};
    Observer *observer{nullptr};

    DataCallbackResult onAudioReady(AudioStream *stream, void *data, int32_t numFrames) override;

    void onErrorBeforeClose(AudioStream *oboeStream, Result error) override;

    void onErrorAfterClose(AudioStream *oboeStream, Result error) override;

    virtual IStream *setupStream() = 0;

public:
    BaseEngine(bool isRecordMode);

    virtual ~BaseEngine();

    void setObserver(Observer *observer);

    virtual void prepare(int sample, const char *musicPath, const char *guidePath, const char *vocalPath);

    virtual void start();

    virtual void stop();

    virtual void resume();

    virtual void pause();

    virtual void setEcho(bool isEcho);

    virtual void seek(int64_t millis);

    virtual void setVolume(float volume,int track);

    virtual void setFilter(int type);

    virtual void setPitch(int pitch);

    virtual int64_t getTotalMs();

    virtual int64_t getCurrentMs();

};


#endif //SUPERPLAYER_BASEENGINE_H
