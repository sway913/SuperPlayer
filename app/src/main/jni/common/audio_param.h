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
    char *function_path{nullptr};
    char *vocal_path{nullptr};
    int out_sample{0};

    void copyPath(char *&dst, const char *src) {
        size_t len = strlen(src);
        if (len > 0) {
            dst = new char[len];
            strcpy(dst, src);
        }
    }

public:

    AudioParam(bool isRecorder, const char *accPath, const char *functionPath, const char *vocalPath, int outSample) {
        this->is_recorder = isRecorder;
        this->out_sample = outSample;
        copyPath(acc_path, accPath);
        copyPath(function_path, functionPath);
        copyPath(vocal_path, vocalPath);
        LOGI("audio param path acc %s\n fun %s\n sample %d", acc_path, function_path, out_sample);
    }

    bool isRecorder() const {
        return is_recorder;
    }

    const char *getAccPath() const {
        return acc_path;
    }

    const char *getFunctionPath() const {
        return function_path;
    }

    const char *getVocalPath() const {
        return vocal_path;
    }

    int getOutSample() const {
        return out_sample;
    }

    virtual ~AudioParam() {
        DELETEARR(acc_path);
        DELETEARR(function_path);
        DELETEARR(vocal_path);
    }
};

#endif //SUPERPLAYER_AUDIO_PARAM_H
