//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_RECORDER_ENGINE_H
#define SUPERPLAYER_RECORDER_ENGINE_H


#include "audio_engine.h"
#include "oboe_recorder.h"

using namespace std;

class RecorderEngine : public AudioEngine {

public:
    RecorderEngine(int sampleRate);

    void prepare(SourceFactory *factory) override final;

    void start() override final;

    void stop() override final;

    void setEcho(bool isEcho) override final;

    DataCallbackResult onAudioReady(AudioStream *stream, void *data, int32_t numFrames) override final;

    void onSourceReady(long total_ms, int index) override final;

private:
    bool mIsThreadAffinitySet{false};
    std::vector<int> mCpuIds; // IDs of CPU cores which the audio callback should be bound to
    OboeRecorder *oboe_recorder{nullptr};
    unique_ptr<short[]> record_buffer{nullptr};

    int getMaxOutNumFrames();

    void setThreadAffinity();

    void disposeReadyState(int index);

    std::mutex mutex;
    bool recorder_ready{false};
    bool source_ready{false};

    bool isEcho{false};

};


#endif //SUPERPLAYER_RECORDER_ENGINE_H
