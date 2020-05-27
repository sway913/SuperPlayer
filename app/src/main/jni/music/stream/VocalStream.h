//
// Created by ldr on 2019-11-18.
//

#ifndef SUPERPLAYER_VOCALSTREAM_H
#define SUPERPLAYER_VOCALSTREAM_H


#include "../engine/recorder/PcmWriter.h"
#include "oboe/Oboe.h"
#include "IStream.h"
#include "../datasource/IDataSource.h"
#include "../common/AudioProperty.h"
#include "../common/Mix.hpp"
#include <vector>

using namespace oboe;

class VocalStream : public IStream {

private:

    PcmWriter *pcmWriter{nullptr};
    AudioStream *inputStream{nullptr};
    AudioStream *outputStream{nullptr};

    std::vector<IDataSource *> sources;
    STATUS status = ERROR;

    int16_t *inputBuffer{nullptr};

    AudioProperty outProperty;

    int64_t currentMs = 0;
    Mix *mix{nullptr};
    int8_t callbackCountToDrain = COUNT_CALLBACK_PREPARE;
    bool isEcho = false;
    AudioFilter * audioFilter{nullptr};
    float volume = 1;
    int filter = 0;
    bool mIsThreadAffinitySet = false;
    std::vector<int> mCpuIds; // IDs of CPU cores which the audio callback should be bound to
    void setThreadAffinity();


public:

    VocalStream();

    virtual ~VocalStream();

    void init(AudioStream *inputStream, AudioStream *outputStream);

    void prepare(const char *musicPath, const char *guidePath, const char *vocalPath) override;

    void start() override;

    void stop() override;

    void resume() override;

    void pause() override;

    void setEcho(bool isEcho) override;

    void seek(int64_t millis) override;

    void setVolume(float volume,int track) override;

    void setFilter(int type) override;

    void setPitch(int pitch) override;

    bool onAudioDataReady(int16_t *data, int32_t numFrames) override;

    void onDataReady() override;

    int64_t getTotalMs() override;

    int64_t getCurrentMs() override;


};


#endif //SUPERPLAYER_VOCALSTREAM_H
