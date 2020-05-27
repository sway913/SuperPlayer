//
// Created by ldr on 2019-11-24.
//

#ifndef SUPERPLAYER_MIX_HPP
#define SUPERPLAYER_MIX_HPP

#include <math.h>
#include <stdlib.h>

class Mix {

    int16_t kInt16MaxValue = static_cast<int16_t>(pow(2, 16 - 1) - 1);
    int32_t kLessMaxValue = static_cast<int32_t>(kInt16MaxValue * 0.95);
    int32_t kMoreMaxValue = static_cast<int32_t>(kInt16MaxValue * 1.05);

public:
    virtual int16_t mixData(int16_t target, int16_t mixData) {
        int32_t tmpData = target + mixData;
        if (tmpData > kMoreMaxValue) {
            tmpData = kInt16MaxValue;
        } else if (tmpData < -kMoreMaxValue) {
            tmpData = -kInt16MaxValue;

        } else if (tmpData > kLessMaxValue) {
            int32_t t = kMoreMaxValue - tmpData;
            tmpData = static_cast<int16_t>(kInt16MaxValue - t);
        } else if (tmpData < -kLessMaxValue) {
            int32_t t = tmpData + kMoreMaxValue;
            tmpData = static_cast<int16_t>(t - kInt16MaxValue);
        } else {
            tmpData = static_cast<int16_t>(tmpData);
        }
        return static_cast<int16_t>(tmpData);
    }
};

#endif //SUPERPLAYER_MIX_HPP
