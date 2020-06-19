//
// Created by ldr on 2020/6/17.
//

#include "filter_package.h"

void FilterPackage::init(Tracker tracker, int sample_rate, int channels) {
    lock_guard<mutex> lock(_mutex);
    if (tracker == Vocal) {
        filters.push_back(new VolumeFilter());
        ff_filter = new FFFilter(sample_rate, channels);
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
        if (auto *volumeFilter = dynamic_cast<VolumeFilter *>(filter)) {
            volumeFilter->setVolume(vol);
        }
    }
}


void FilterPackage::setPitch(float pitch) {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter : filters) {
        if (auto *pitchFilter = dynamic_cast<PitchFilter *>(filter)) {
            pitchFilter->setPitch(pitch);
        }
    }
}

void FilterPackage::setEffect(int type) {
    lock_guard<mutex> lock(_mutex);
    if (type != 15) {
        if (ff_filter) {
            ff_filter->setFilter(type);
        }
    } else {

    }
    filter_type = type;
}

int FilterPackage::process(short *data, int len) {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter : filters) {
        filter->process(data, len);
    }
    if (filter_type != 14) {
        if (filter_type != 15) {
            ff_filter->process(data, len);
        } else {

        }
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