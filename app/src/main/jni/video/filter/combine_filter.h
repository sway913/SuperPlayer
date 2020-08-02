//
// Created by Lvlingling on 2020/7/4.
//

#ifndef SUPERPLAYER_COMBINE_FILTER_H
#define SUPERPLAYER_COMBINE_FILTER_H


#include "v_filter.h"
#include "../utils/video_effect.h"
#include <list>
#include <mutex>
#include "smooth_filter.h"
#include "white_filter.h"
#include "lookup_filter.h"

using namespace std;

class CombineFilter : public VFilter {

public:

    CombineFilter();

    virtual GLuint draw(GLuint textureId, int w, int h) override ;

    virtual void destroy() override;

    void setEffect(std::shared_ptr<VideoEffect> &effect);

    virtual ~CombineFilter();

private:

    list<VFilter *> filters{};

    mutex _mutex;
};


#endif //SUPERPLAYER_COMBINE_FILTER_H
