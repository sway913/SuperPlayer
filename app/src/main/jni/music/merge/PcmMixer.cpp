
#include "PcmMixer.h"
#include "../common/AndroidLog.h"
#include "../common/CommonTools.h"
#include <jni.h>

PcmMixer::PcmMixer() {

}

void PcmMixer::mix(int8_t *recBuffer, int8_t *musicBuffer, int size, int8_t *out) {

    if (vocal_buf == NULL || bufSize != size) {
        if (vocal_buf) {
            delete vocal_buf;
        }
        vocal_buf = (int16_t *) malloc(static_cast<size_t>(size));
        bufSize = size;
    }

    if (music_buf == NULL || bufSize != size) {
        if (music_buf) {
            delete music_buf;
        }
        music_buf = (int16_t *) malloc(static_cast<size_t>(size));
        bufSize = size;
    }

    for (int i = 0; i < size / 2; i++) {
        const uint8_t *in1 = (uint8_t *) &recBuffer[2 * i];
        vocal_buf[i] = in1[0] | (in1[1] << 8);

        const uint8_t *in2 = (uint8_t *) &musicBuffer[2 * i];
        music_buf[i] = in2[0] | (in2[1] << 8);
    }

    mixtureAccompanyAudio(vocal_buf, music_buf, size/2, (byte *) out);//人声伴奏合成
}

void PcmMixer::mix(int16_t *recBuffer, int16_t *musicBuffer, int size, int16_t *out, float musicFactorLeft,
                   float musicFactorRight, float recFactor) {
    int16_t musicLeft, musicRight, musicData, recData;
    int32_t output;
    for (int i = 0; i < size; i++) {
        musicLeft = (int16_t) ((musicBuffer[4 * i] & 0xff) | ((musicBuffer[4 * i + 1] & 0xff) << 8));
        musicRight = (int16_t) ((musicBuffer[4 * i + 2] & 0xff) | ((musicBuffer[4 * i + 3] & 0xff) << 8));
        recData = (int16_t) ((recBuffer[2 * i] & 0xff) | ((recBuffer[2 * i + 1] & 0xff) << 8));

        musicData = (int16_t) ((musicBuffer[2 * i] & 0xff) | ((musicBuffer[2 * i + 1] & 0xff) << 8));

        /*output = (int32_t) ((musicLeft * musicFactorLeft * LEFT_MUSIC_MAX_VOLUME_FACTOR
                             + musicRight * musicFactorRight * RIGHT_MUSIC_MAX_VOLUME_FACTOR
                             + recData * recFactor * RECORD_MAX_VOLUME_FACTOR) * f);*/
        output = (int32_t) ((musicData * musicFactorLeft * RECORD_MAX_VOLUME_FACTOR
                             + recData * recFactor * RECORD_MAX_VOLUME_FACTOR) * f);

        if (output > MAX) {
            f = (double) MAX / (double) (output);
            output = MAX;
        }
        if (output < MIN) {
            f = (double) MIN / (double) (output);
            output = MIN;
        }
        if (f < 1) {
            f += ((double) 1 - f) / (double) 32;
        }

        out[2 * i] = (jbyte) (output & 0xff);
        out[2 * i + 1] = (jbyte) ((output >> 8) & 0xff);
    }
}

void PcmMixer::release() {
    if (vocal_buf) {
        delete vocal_buf;
        vocal_buf = NULL;
    }

    if (music_buf) {
        delete music_buf;
        music_buf = NULL;
    }
    bufSize = 0;
    LOGI("PcmMixer release");
}

PcmMixer::~PcmMixer() {
    release();
}