//
// Created by Lvlingling on 2020-06-04.
//

#ifndef SUPERPLAYER_AUDIO_PARAM_H
#define SUPERPLAYER_AUDIO_PARAM_H

#include "common_tools.h"
#include <string>

class AudioParam {

private:

    bool is_recorder{false};
    char *acc_path{nullptr};
    char *guide_path{nullptr};
    char *vocal_path{nullptr};
    char *decode_path{nullptr};
    int out_sample{0};

    static inline void copyPath(char *&dst, const char *src) {
        size_t len = strlen(src);
        if (len > 0) {
            dst = new char[len];
            strcpy(dst, src);
        }
    }

public:

    AudioParam(bool isRecorder, int outSample, const char *accPath, const char *guidePath, const char *vocalPath, const char *decodePath) {
        this->is_recorder = isRecorder;
        this->out_sample = outSample;
        copyPath(acc_path, accPath);
        copyPath(guide_path, guidePath);
        copyPath(vocal_path, vocalPath);
        copyPath(decode_path, decodePath);
    }

    bool isRecorder() const {
        return is_recorder;
    }

    const char *getAccPath() const {
        return acc_path;
    }

    const char *getGuidePath() const {
        return guide_path;
    }

    const char *getVocalPath() const {
        return vocal_path;
    }

    const char *getDecodePath() const {
        return decode_path;
    }

    int getOutSample() const {
        return out_sample;
    }

    virtual ~AudioParam() {
        DELETEARR(acc_path)
        DELETEARR(guide_path)
        DELETEARR(vocal_path)
        DELETEARR(decode_path)
    }
};

#endif //SUPERPLAYER_AUDIO_PARAM_H
