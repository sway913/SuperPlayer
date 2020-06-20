//
// Created by ldr on 2020/6/17.
//

#include "filter_package.h"

void FilterPackage::init(Tracker tracker, int sample_rate, int channels) {
    lock_guard<mutex> lock(_mutex);
    if (tracker == Vocal) {
        filters.push_back(new VolumeFilter());
        filters.push_back(new FFFilter());
        filters.push_back(new CustomFilter());
    }
    if (tracker == Acc) {
        filters.push_back(new VolumeFilter());
        filters.push_back(new PitchFilter());
    }
    for (auto &filter : filters) {
        filter->init(sample_rate, channels);
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

void FilterPackage::setEffect(float *arr) {
    lock_guard<mutex> lock(_mutex);
    int index = (int) arr[8];
    if (index == 14) {
        filter_type = None;
    } else if (index == 15) {
        filter_type = Custom;
        for (auto &filter : filters) {
            if (auto *custom_filter = dynamic_cast<CustomFilter *>(filter)) {
                custom_filter->setEffect(arr);
            }
        }
    } else {
        filter_type = FF_Filter;
        for (auto &filter : filters) {
            if (auto *ff_filter = dynamic_cast<FFFilter *>(filter)) {
                ff_filter->setFilter(index);
            }
        }
    }
}

int FilterPackage::process(short *data, int len) {
    lock_guard<mutex> lock(_mutex);
    for (auto &filter : filters) {
        if (auto *custom_filter = dynamic_cast<CustomFilter *>(filter)) {
            if (filter_type == Custom) {
                custom_filter->process(data, len);
            }
        } else if (auto *ff_filter = dynamic_cast<FFFilter *>(filter)) {
            if (filter_type == FF_Filter) {
                ff_filter->process(data, len);
            }
        } else {
            filter->process(data, len);
        }
        if (filter_type == None) {

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