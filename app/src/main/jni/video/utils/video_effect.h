//
// Created by ldr on 2020/7/1.
//

#ifndef SUPERPLAYER_VIDEO_EFFECT_H
#define SUPERPLAYER_VIDEO_EFFECT_H

#include "../../common/common_tools.h"

class VideoEffect {

    static inline void copyPath(char *&dst, const char *src) {
        size_t len = strlen(src);
        if (len > 0) {
            dst = new char[len];
            strcpy(dst, src);
        }
    }

public:

    VideoEffect(float w_level, float s_level, const char *t_path) {
        this->whiteLevel = w_level;
        this->smoothLevel = s_level;
        copyPath(table_path, t_path);
    }

    float getWhiteLevel() const {
        return whiteLevel;
    }

    float getSmoothLevel() const {
        return smoothLevel;
    }

    char *getTablePath() const {
        return table_path;
    }

    virtual ~VideoEffect() {
        DELETEARR(table_path)
    }

private:
    float whiteLevel = 0;
    float smoothLevel = 0;
    char *table_path{nullptr};

};

#endif //SUPERPLAYER_VIDEO_EFFECT_H
