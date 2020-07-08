//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_RENDER_H
#define SUPERPLAYER_RENDER_H


#include <memory>
#include "source/source.h"
#include "filter/ifilter.h"

class Render {

public:

    void onSurfaceCreate(int w, int h);

    void onDraw();

    void onSurfaceDestroy();

    void setSource(Source *s);

    void setFilter(const std::shared_ptr<IFilter> &f);


private:

    Source *source{nullptr};

    std::shared_ptr<IFilter> filter{nullptr};
    IFilter *outRenderFilter{nullptr};

    int width{0};
    int height{0};

};

#endif //SUPERPLAYER_RENDER_H
