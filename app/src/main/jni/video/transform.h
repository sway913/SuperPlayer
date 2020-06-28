//
// Created by Lvlingling on 2020/6/27.
//

#ifndef SUPERPLAYER_TRANSFORM_H
#define SUPERPLAYER_TRANSFORM_H

#include "../common/matrix.h"
#include "../common/common_tools.h"

class Transform {

public:

    static void cropViewport(float *matrix, int src_w, int src_h, int dst_w, int dst_h) {
        if (src_h > 0 && src_w > 0 && dst_w > 0 && dst_h > 0) {
            float dst_ratio = (float) dst_w / (float) dst_h;
            float src_ratio = (float) src_w / (float) src_h;
            if (src_ratio > dst_ratio) {
                float ratio = src_ratio / dst_ratio;
                matrixScaleM(matrix, ratio, 1, 1);
            } else {
                float ratio = dst_ratio / src_ratio;
                matrixScaleM(matrix, 1, ratio, 1);
            }
        }
    }
};


#endif //SUPERPLAYER_TRANSFORM_H
