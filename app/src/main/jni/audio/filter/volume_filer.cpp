//
// Created by ldr on 2020/6/17.
//

#include "volume_filer.h"

void VolumeFilter::init(int sampleRate, int channel, int param) {

}

int VolumeFilter::process(short *data, int len) {
    if (volume == 1.0) {
        return len;
    }
    adjustSamplesVolume(data, len, volume);
    return len;
}

void VolumeFilter::setVolume(float vol) {
    this->volume = vol;
}

void VolumeFilter::destroy() {

}

FilterType VolumeFilter::getType() {
    return FilterType::VolumeType;
}