//
// Created by ldr on 2019-12-16.
//

#include "FilterPackage.h"

FilterPackage::FilterPackage(AudioProperty outProperty) {
    this->outProperty = outProperty;
    pSoundTouch = new soundtouch::SoundTouch();
    pSoundTouch->setChannels(static_cast<uint>(outProperty.channelCount));
    pSoundTouch->setSampleRate(static_cast<uint>(outProperty.sampleRate));
    pSoundTouch->setPitchSemiTones(0.0);
    pAudioFilter = new AudioFilter(outProperty);
}


size_t FilterPackage::process(void *data, int32_t size) {
    size_t len = 0;
    if (pSoundTouch->getVirtualPitch() != 1) {
        pSoundTouch->putSamples(reinterpret_cast<const soundtouch::SAMPLETYPE *>(data), static_cast<uint>(size / 2 / outProperty.channelCount));
        len = pSoundTouch->receiveSamples(reinterpret_cast<soundtouch::SAMPLETYPE *>(data), static_cast<uint>(size / 2 / outProperty.channelCount));
    }

    if (volume != 1 || filter != EFFECT_PRESET_NONE) {
        pAudioFilter->process(static_cast<char *>(data), size, size / outProperty.channelCount / sizeof(int16_t));
    }

    return len;
}


void FilterPackage::setPitch(int pitch) {
    pSoundTouch->setPitchSemiTones((double(pitch)));
}

void FilterPackage::setFilter(int type) {
    this->filter = type;
    //  pAudioFilter->setFilter(type);
    pAudioFilter->setFilter(type);
}

void FilterPackage::setVolume(double volume) {
    this->volume = volume;
    pAudioFilter->setVolume(volume);
}

FilterPackage::~FilterPackage() {
    if (pSoundTouch) {
        delete pSoundTouch;
        pSoundTouch = nullptr;
    }
    if (pAudioFilter) {
        delete pAudioFilter;
        pAudioFilter = nullptr;
    }
}
