//
// Created by ldr on 2019-12-03.
//

#ifndef SUPERPLAYER_AAC_ENCODER_H
#define SUPERPLAYER_AAC_ENCODER_H

#include <cstdint>
#include <fdk-aac/aacenc_lib.h>


class AacEncoder {

public:

    AacEncoder();

    virtual ~AacEncoder();

    void init(int32_t sampleRate, int channels, int32_t bitRate);

    int encode(int8_t *input, int8_t *output, int size);

private:

    HANDLE_AACENCODER *aacencoder{nullptr};
    AACENC_InfoStruct *aacencInfoStruct{nullptr};
    int16_t *convertBuffer{nullptr};
    int convertBufferSize = 0;
};


#endif //SUPERPLAYER_AAC_ENCODER_H
