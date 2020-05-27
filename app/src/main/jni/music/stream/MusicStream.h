//
// Created by ldr on 2019-11-18.
//


#include <vector>
#include "../datasource/PcmDataSource.h"
#include "IStream.h"
#include "../decode/FFDecoder.h"
#include "../decode/FileDecoder.h"
#include "../common/Mix.hpp"

class MusicStream : public IStream {

private:
    std::vector<IDataSource *> sources;
    STATUS status = ERROR;
    AudioStream *outputStream{nullptr};
    FileDecoder *fileDecoder{nullptr};
    int64_t totalMs = 0;
    int64_t currentMs = 0;
    Mix *mix{nullptr};
    char *decodePath{nullptr};
    int32_t vocalFileLength = 0;

public:

    MusicStream();

    virtual ~MusicStream();

    void prepare(const char *musicPath, const char *decodeFilePath, const char *vocalPath) override;

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

    void setOutputStream(AudioStream *outputStream);

    void onDecodeCompleted();

    void truncateFile();

};


