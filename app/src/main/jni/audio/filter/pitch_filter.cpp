//
// Created by Lvlingling on 2020/6/17.
//

#include "pitch_filter.h"

void PitchFilter::init(int sampleRate, int channel) {
    soundTouch = new SoundTouch();
    soundTouch->setSampleRate(static_cast<uint>(sampleRate));
    soundTouch->setChannels(static_cast<uint>(channel));
    soundTouch->setPitchSemiTones(pitch_shift);
    soundTouch->setSetting(SETTING_SEEKWINDOW_MS, 0);
    soundTouch->setSetting(SETTING_SEQUENCE_MS, 0);
    soundTouch->setSetting(SETTING_OVERLAP_MS, 8);
    soundTouch->setSetting(SETTING_USE_QUICKSEEK, 1);
    this->channels = channel;
}

int PitchFilter::process(short *data, int len) {
    if (pitch_shift != 0) {
        soundTouch->putSamples(data, static_cast<uint>(len / channels));
        soundTouch->receiveSamples(data, static_cast<uint>(len / channels));
    }
    return len;
}

void PitchFilter::destroy() {
    delete soundTouch;
    soundTouch = nullptr;
}

void PitchFilter::setPitch(float pitch) {
    soundTouch->setPitchSemiTones(pitch);
    this->pitch_shift = pitch;
}