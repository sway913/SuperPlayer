//
// Created by Lvlingling on 2020/6/25.
//

#include "render.h"
#include "../common/common_tools.h"
#include "filter/out_filter.h"
#include "../common/android_log.h"

void Render::onSurfaceCreate(int w, int h) {
    outRenderFilter = new OutFilter();
    this->width = w;
    this->height = h;
}


void Render::onDraw() {
    GLuint textureId;
    if (source) {
        textureId = source->produceFrame();
        if (filter) {
            textureId = filter->draw(textureId, width, height);
        }
        outRenderFilter->draw(textureId, width, height);
    }
}


void Render::onSurfaceDestroy() {
    outRenderFilter->destroy();
    DELETEOBJ(outRenderFilter)
    if (filter) {
        filter->destroy();
        filter = nullptr;
    }
}

void Render::setSource(Source *s) {
    this->source = s;
}

void Render::setFilter(const std::shared_ptr<VFilter> &f) {
    this->filter = f;
}
