//
// Created by kaileiwang on 2019/6/24.
//

#ifndef SUPERPLAYER_PCMMIXUER_H
#define SUPERPLAYER_PCMMIXUER_H

/** 经验值，用于适当放大录音音量 */
#define RECORD_MAX_VOLUME_FACTOR 1
#define LEFT_MUSIC_MAX_VOLUME_FACTOR 0.5
#define RIGHT_MUSIC_MAX_VOLUME_FACTOR 0.5


#include <cstdint>
#include <cstdlib>

class PcmMixer {
public:
    int const MAX = 32767;
    int const MIN = -32768;
    double f = 1.0;

    int16_t *vocal_buf = NULL;
    int16_t *music_buf = NULL;
    int bufSize = 0;


    PcmMixer();

    void mix(int8_t *recBuffer, int8_t *musicBuffer, int size, int8_t *out);

    void mix(int16_t *recBuffer, int16_t *musicBuffer, int size, int16_t *out, float musicFactorLeft, float musicFactorRight, float recFactor);

    void release();

    ~PcmMixer();
};

#endif //SUPERPLAYER_PCMMIXUER_H
