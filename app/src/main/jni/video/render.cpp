//
// Created by Lvlingling on 2020/6/25.
//

#include "render.h"
#include "../common/common_tools.h"
#include "filter/out_filter.h"

void Render::onSurfaceCreate(int w, int h) {
    outRenderFilter = new OutFilter();
    this->width = w;
    this->heigth = h;
}


void Render::onDraw() {
    GLuint textureId;
    if (source) {
        textureId = source->produceFrame();
//        if (filter) {
//            textureId = filter->draw(textureId, width, heigth);
//        }
        outRenderFilter->draw(textureId, width, heigth);
    }
}


void Render::onSurfaceDestroy() {
    outRenderFilter->destroy();
    DELETEOBJ(outRenderFilter)
}

void Render::setSource(Source *s) {
    this->source = s;
}

void Render::setFilter(const std::shared_ptr<IFilter> &f) {
    this->filter = f;
}
