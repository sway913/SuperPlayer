//
// Created by Lvlingling on 2020/7/4.
//

#include "combine_filter.h"

CombineFilter::CombineFilter() {
    auto *smoothFilter = new SmoothFilter();
    auto *whiteFilter = new WhiteFilter();
    auto *lookupFilter = new LookupFilter();
    filters.push_back(smoothFilter);
    filters.push_back(whiteFilter);
    filters.push_back(lookupFilter);
}


GLuint CombineFilter::draw(GLuint textureId, int w, int h) {
    lock_guard<mutex> lock(_mutex);
    GLuint ret = textureId;
    for (auto &filter:filters) {
        ret = filter->draw(ret, w, h);
    }
    return ret;
}

void CombineFilter::destroy() {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter:filters) {
        filter->destroy();
    }
    filters.clear();
}

void CombineFilter::setEffect(std::shared_ptr<VideoEffect> &effect) {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter:filters) {
        if (auto *f = dynamic_cast<WhiteFilter *>(filter)) {
            f->setWhiteLevel(effect->getWhiteLevel());
        }
        if (auto *f = dynamic_cast<SmoothFilter *>(filter)) {
            f->setSmoothLevel(effect->getSmoothLevel());
        }
        if (auto *f = dynamic_cast<LookupFilter *>(filter)) {
            f->setTablePath(effect->getTablePath());
        }
    }
}

CombineFilter::~CombineFilter() = default;
