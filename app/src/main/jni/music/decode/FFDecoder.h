//
// Created by ldr on 2019-11-20.
//

#ifndef SUPERPLAYER_FFDECODER_H
#define SUPERPLAYER_FFDECODER_H

#include "../common/AudioProperty.h"
#include "../common/Resample.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "../common/AudioData.h"
}

class FFDecoder {

private:

    AVFormatContext *pFormatContext{nullptr};
    int audioIndex;
    AVCodecContext *pCodecContext{nullptr};
    AVFrame *pAvFrame{nullptr};
    AVPacket *pAvPacket{nullptr};
    AVCodecParameters *pCodecParameters{nullptr};

    int64_t totalTime;
    int bufferLen = 0;

    Resample *resample;
    bool needResample;
    AudioProperty outProperty;

public:

    FFDecoder();

    virtual ~FFDecoder();

    bool prepare(const char *fileName, AudioProperty outProperty);

    AudioData decodeFrame();

    int decodeFrame(uint8_t *&output);

    int64_t getTotalMs();

    void seek(int64_t millis);

    void close();

};


#endif //SUPERPLAYER_FFDECODER_H
