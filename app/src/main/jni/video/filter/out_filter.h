//
// Created by Lvlingling on 2020/6/26.
//

#ifndef SUPERPLAYER_OUT_FILTER_H
#define SUPERPLAYER_OUT_FILTER_H

#include "base_filter.h"

class OutFilter : public BaseFilter {

public:

    OutFilter();

    void bindFrameBuffer(int w, int h) override;

};


#endif //SUPERPLAYER_OUT_FILTER_H
