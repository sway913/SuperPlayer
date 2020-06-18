//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_COMMON_TOOLS_H
#define SUPERPLAYER_COMMON_TOOLS_H

#include <future>

#define DELETEOBJ(obj) if(obj != nullptr) { delete obj; obj = nullptr; }
#define DELETEARR(arr) if(arr != nullptr) { delete[] arr; arr = nullptr; }

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))


inline void JOIN(std::future<void> &result) {
    std::future_status futureStatus;
    do {
        futureStatus = result.wait_for(std::chrono::milliseconds(20));
    } while (futureStatus != std::future_status::ready);
}

static inline short mixAudioSample(short a, short b) {
    int tmp = a < 0 && b < 0 ? ((int) a + (int) b) - (((int) a * (int) b) / INT16_MIN) : (a > 0 && b > 0 ? ((int) a + (int) b) - (((int) a * (int) b) / INT16_MAX) : a + b);
    return static_cast<int16_t>(tmp > INT16_MAX ? INT16_MAX : (tmp < INT16_MIN ? INT16_MIN : tmp));
}

static inline void mixAudioBuffer(short *a, short *b, int len) {
    for (int i = 0; i < len; ++i) {
        int tmp = a[i] < 0 && b[i] < 0 ? ((int) a[i] + (int) b[i]) - (((int) a[i] * (int) b[i]) / INT16_MIN) : (a[i] > 0 && b[i] > 0 ? ((int) a[i] + (int) b[i]) - (((int) a[i] * (int) b[i]) / INT16_MAX) : a[i] + b[i]);
        a[i] =  static_cast<int16_t>(tmp > INT16_MAX ? INT16_MAX : (tmp < INT16_MIN ? INT16_MIN : tmp));
    }
}

inline int16_t adjustAudioVolume(int16_t source, float volume) {

    int16_t result;
    int temp = (int) ((int) source * volume);
    int sign = 1;
    if (temp < 0) {
        sign = -1;
    }
    int abstemp = abs(temp);
    if (abstemp < 29490) {
        result = temp;
    } else if (abstemp > 46285) {
        result = 32767 * sign;
    } else if (abstemp > 32630) {
        result = ((abstemp - 29490) / 8 + 30668) * sign;
    } else {
        result = ((abstemp - 29490) / 2 + 29490) * sign;
    }
    return result;
}


inline void adjustSamplesVolume(int16_t *samples, int size, float accompanyVolume) {
    if (accompanyVolume != 1.0) {
        for (int i = 0; i < size; i++) {
            samples[i] = adjustAudioVolume(samples[i], accompanyVolume);
        }
    }
}


#endif //SUPERPLAYER_COMMON_TOOLS_H
