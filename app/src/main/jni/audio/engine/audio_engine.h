//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_AUDIO_ENGINE_H
#define SUPERPLAYER_AUDIO_ENGINE_H

#include <oboe/Oboe.h>
#include "../observer.h"
#include "../source/source_factory.h"

using namespace oboe;

class AudioEngine : public AudioStreamCallback {

protected:

    int sample_rate{48000};

    ManagedStream output_stream{nullptr};
    Observer *observer{nullptr};

    DataCallbackResult onAudioReady(AudioStream *stream, void *data, int32_t numFrames) override;

    void onErrorBeforeClose(AudioStream *oboeStream, Result error) override;

    void onErrorAfterClose(AudioStream *oboeStream, Result error) override;

    bool openStream();


public:

    AudioEngine(int sampleRate);

    void setObserver(Observer *observer);

    virtual void prepare(SourceFactory *factory) = 0;

    virtual void start() = 0;

    virtual void stop();

    virtual void resume();

    virtual void pause();

    virtual void setEcho(bool isEcho);

    virtual void seek(int64_t millis);

    virtual void setVolume(float volume, int track);

    virtual void setFilter(int type);

    virtual void setPitch(int pitch);

    virtual int64_t getTotalMs();

    virtual int64_t getCurrentMs();
};


#endif //SUPERPLAYER_AUDIO_ENGINE_H
