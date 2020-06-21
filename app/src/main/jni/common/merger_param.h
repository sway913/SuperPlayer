//
// Created by Lvlingling on 2020/6/12.
//

#ifndef SUPERPLAYER_MERGER_PARAM_H
#define SUPERPLAYER_MERGER_PARAM_H

#include <string>
#include "common_tools.h"

class MergerParam {
private:

    char *acc_path{nullptr};
    char *vocal_path{nullptr};
    char *out_path{nullptr};
    float vocal_volume{1.0f};
    float acc_volume{1.0f};
    float pitch{0.0f};
    float *effect{nullptr};

    static inline void copyPath(char *&dst, const char *src) {
        size_t len = strlen(src);
        if (len > 0) {
            dst = new char[len];
            strcpy(dst, src);
        }
    }

public:

    MergerParam(const char *accPath, const char *vocalPath, const char *outPath, float vocalVolume,
                float accVolume, float pitch_, float *arr) {
        copyPath(acc_path, accPath);
        copyPath(vocal_path, vocalPath);
        copyPath(out_path, outPath);
        this->vocal_volume = vocalVolume;
        this->acc_volume = accVolume;
        this->pitch = pitch_;
        effect = new float[9];
        for (int i = 0; i < 9; ++i) {
            effect[i] = arr[i];
        }
    }

    const char *getAccPath() const {
        return acc_path;
    }

    const char *getVocalPath() const {
        return vocal_path;
    }

    const char *getOutPath() const {
        return out_path;
    }

    float getVocalVolume() const {
        return vocal_volume;
    }

    float getAccVolume() const {
        return acc_volume;
    }

    float getPitch() const {
        return pitch;
    }

    float *getEffect() const {
        return effect;
    }

    virtual ~MergerParam() {
        DELETEARR(acc_path)
        DELETEARR(vocal_path)
        DELETEARR(out_path)
        DELETEARR(effect)
    }
};

#endif //SUPERPLAYER_MERGER_PARAM_H
