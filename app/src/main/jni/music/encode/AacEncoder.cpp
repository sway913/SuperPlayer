//
// Created by ldr on 2019-12-03.
//

#include <cstdlib>
#include "AacEncoder.h"
#include "../common/AndroidLog.h"
#include <memory>

AacEncoder::AacEncoder() {}

void AacEncoder::init(int32_t sampleRate, int channels, int32_t bitRate) {
    CHANNEL_MODE mode;
    switch (channels) {
        case 1:
            mode = MODE_1;
            break;
        case 2:
            mode = MODE_2;
            break;
        case 3:
            mode = MODE_1_2;
            break;
        case 4:
            mode = MODE_1_2_1;
            break;
        case 5:
            mode = MODE_1_2_2;
            break;
        case 6:
            mode = MODE_1_2_2_1;
            break;
        default:
            mode = MODE_1;
            break;
    }
    int ret = 0;
    aacencoder = new HANDLE_AACENCODER();
    aacencInfoStruct = new AACENC_InfoStruct();
    memset(aacencInfoStruct, 0, sizeof(AACENC_InfoStruct));
    ret = aacEncOpen(aacencoder, 0, static_cast<const UINT>(channels));
    if (ret != AACENC_OK) {
        LOGI("open aac fail");
        return;
    }

    ret = aacEncoder_SetParam(*aacencoder, AACENC_AOT, 2);
    if (ret != AACENC_OK) {
        LOGI("aac param aot fail");
        return;
    }

    ret = aacEncoder_SetParam(*aacencoder, AACENC_SAMPLERATE, static_cast<const UINT>(sampleRate));
    if (ret != AACENC_OK) {
        LOGI("aac samplerate fail");
    }

    ret = aacEncoder_SetParam(*aacencoder, AACENC_CHANNELMODE, mode);
    if (ret != AACENC_OK) {
        LOGI("aac channelmode fail");
        return;
    }

    ret = aacEncoder_SetParam(*aacencoder, AACENC_BITRATE, static_cast<const UINT>(bitRate));
    if (ret != AACENC_OK) {
        LOGI("aac bitrate fail");
        return;
    }

    ret = aacEncEncode(*aacencoder, nullptr, nullptr, nullptr, nullptr);
    if (ret != AACENC_OK) {
        LOGI("aac encode open fail");
        return;
    }

    ret = aacEncInfo(*aacencoder, aacencInfoStruct);
    if (ret != AACENC_OK) {
        LOGI("aac info get fail");
        return;
    }
    LOGI("aac init success");
}

int AacEncoder::encode(int8_t *input, int8_t *output, int size) {

    int8_t *data1 = input;
    int8_t *data2 = output;

    int input_size = size;
    int output_size = size;

    int encodeSize = 0;
    AACENC_BufDesc inBuffer = {0};
    AACENC_BufDesc outBuffer = {0};
    AACENC_InArgs inArgs = {0};
    AACENC_OutArgs outArgs = {0};
    int in_identifier = IN_AUDIO_DATA;
    int in_elem_size;
    int out_identifier = OUT_BITSTREAM_DATA;
    int out_elem_size;
    AACENC_ERROR err;

    if (aacencoder == nullptr) {
        LOGI("aacencoder is null");
        return -1;
    }

    if (convertBuffer == nullptr || convertBufferSize != input_size) {
        convertBuffer = static_cast<int16_t *>(realloc(convertBuffer, static_cast<size_t>(input_size)));
        convertBufferSize = size;
    }

    for (int i = 0; i < input_size / 2; i++) {
        const uint8_t *in = (uint8_t *) &data1[2 * i];
        convertBuffer[i] = in[0] | (in[1] << 8);
    }

    in_elem_size = 2;

    inArgs.numInSamples = input_size <= 0 ? -1 : input_size / 2;
    inBuffer.numBufs = 1;
    inBuffer.bufs = (void **) &convertBuffer;
    inBuffer.bufferIdentifiers = &in_identifier;
    inBuffer.bufSizes = &input_size;
    inBuffer.bufElSizes = &in_elem_size;

    out_elem_size = 1;

    outBuffer.numBufs = 1;
    outBuffer.bufs = (void **) &data2;
    outBuffer.bufferIdentifiers = &out_identifier;
    outBuffer.bufSizes = &output_size;
    outBuffer.bufElSizes = &out_elem_size;


    if ((err = aacEncEncode(*aacencoder, &inBuffer, &outBuffer, &inArgs, &outArgs)) != AACENC_OK) {
        encodeSize = 0;
        if (err == AACENC_ENCODE_EOF) {
            LOGI("encode finish");
            encodeSize = -1;
        }
    } else {
        encodeSize = outArgs.numOutBytes;
    }
    return encodeSize;

}

AacEncoder::~AacEncoder() {
    if (aacencoder) {
        delete aacencoder;
        aacencoder = nullptr;
    }

    if (aacencInfoStruct) {
        delete aacencInfoStruct;
        aacencInfoStruct = nullptr;
    }

    if (convertBuffer) {
        delete convertBuffer;
        convertBuffer = nullptr;
    }
}
