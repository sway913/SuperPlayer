//
// Created by ldr on 2020/6/17.
//

#include "filter_package.h"

void FilterPackage::init(Tracker tracker, int sample_rate, int channels) {
    lock_guard<mutex> lock(_mutex);
    if (tracker == Vocal) {
        filters.push_back(new VolumeFilter());
    }
    if (tracker == Acc) {
        filters.push_back(new VolumeFilter());
        filters.push_back(new PitchFilter());
    }
    for (auto &filter : filters) {
        filter->init(sample_rate, channels, 0);
    }
}

void FilterPackage::setVolume(float vol) {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter : filters) {
        if (filter->getType() == VolumeType) {
            ((VolumeFilter *) filter)->setVolume(vol);
        }
    }
}


void FilterPackage::setPitch(float pitch) {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter : filters) {
        if (filter->getType() == PitchType) {
            ((PitchFilter *) filter)->setPitch(pitch);
        }
    }
}

void FilterPackage::setEffect(int type) {
    lock_guard<mutex> lock(_mutex);

}

int FilterPackage::process(short *data, int len) {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter : filters) {
        filter->process(data, len);
    }
    return len;
}

void FilterPackage::destroy() {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter : filters) {
        filter->destroy();
        delete filter;
    }
    filters.clear();
}