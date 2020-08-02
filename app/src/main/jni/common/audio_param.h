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
    bool withVideo{false};
    char *video_path{nullptr};

    static inline void copyPath(char *&dst, const char *src) {
        size_t len = strlen(src);
        if (len > 0) {
            dst = new char[len];
            strcpy(dst, src);
        }
    }

public:

    AudioParam(bool isRecorder, int outSample, const char *accPath, const char *guidePath, const char *vocalPath,
               const char *decodePath, bool with_video, const char *videoPath) {
        this->is_recorder = isRecorder;
        this->out_sample = outSample;
        this->withVideo = with_video;
        copyPath(acc_path, accPath);
        copyPath(guide_path, guidePath);
        copyPath(vocal_path, vocalPath);
        copyPath(decode_path, decodePath);
        copyPath(video_path, videoPath);
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

    bool isWithVideo() const {
        return withVideo;
    }

    char *getVideoPath() const {
        return video_path;
    }

    virtual ~AudioParam() {
        DELETEARR(acc_path)
        DELETEARR(guide_path)
        DELETEARR(vocal_path)
        DELETEARR(decode_path)
        DELETEARR(video_path)
    }
};

#endif //SUPERPLAYER_AUDIO_PARAM_H
