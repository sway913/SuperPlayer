//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_AUDIO_ENGINE_H
#define SUPERPLAYER_AUDIO_ENGINE_H

#include <oboe/Oboe.h>
#include <vector>
#include "../observer.h"
#include "../source/source_factory.h"
#include "../source/mix_source.h"
#include "../filter/filter_package.h"

using namespace oboe;
using namespace std;
using namespace placeholders;

class AudioEngine : public AudioStreamCallback {

protected:

    int sample_rate{48000};
    ManagedStream out_stream{nullptr};
    Observer *observer{nullptr};
    SourceFactory *source_factory{nullptr};
    vector<ISource *> source{};
    MixSource *mix_source{nullptr};
    long total_ms{0};
    bool is_pause{false};
    volatile std::atomic<bool> stopped{false};
    FilterPackage *vocal_filter{nullptr};
    FilterPackage *acc_filter{nullptr};


    DataCallbackResult onAudioReady(AudioStream *stream, void *data, int32_t numFrames) override;

    void onErrorBeforeClose(AudioStream *oboeStream, Result error) override;

    void onErrorAfterClose(AudioStream *oboeStream, Result error) override;

    bool openOutputStream();

public:

    AudioEngine(int sampleRate);

    virtual ~AudioEngine();

    virtual void prepare(SourceFactory *factory) = 0;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void onSourceReady(long total_ms, int index) = 0;

public:

    virtual void resume();

    virtual void pause();

    virtual void setEcho(bool isEcho);

    virtual void seek(int64_t millis);

    virtual void setVolume(float volume, int track);

    virtual void setFilter(int type);

    virtual void setCustomFilter(float *arr);

    virtual void setPitch(float pitch);

    virtual int64_t getTotalMs();

    virtual int64_t getCurrentMs();

    void setObserver(Observer *observer);

    static std::unique_ptr<AudioEngine> getEngine(bool is_recorder, int sample_rate);
};


#endif //SUPERPLAYER_AUDIO_ENGINE_H
