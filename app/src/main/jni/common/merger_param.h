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

    static inline void copyPath(char *&dst, const char *src) {
        size_t len = strlen(src);
        if (len > 0) {
            dst = new char[len];
            strcpy(dst, src);
        }
    }

public:

    MergerParam(const char *accPath, const char *vocalPath, const char *outPath) {
        copyPath(acc_path, accPath);
        copyPath(vocal_path, vocalPath);
        copyPath(out_path, outPath);
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

    virtual ~MergerParam() {
        DELETEARR(acc_path)
        DELETEARR(vocal_path)
        DELETEARR(out_path)
    }
};

#endif //SUPERPLAYER_MERGER_PARAM_H
