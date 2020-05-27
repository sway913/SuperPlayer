//
// Created by ldr on 2019-12-16.
//

#ifndef SUPERPLAYER_FILTERPACKAGE_H
#define SUPERPLAYER_FILTERPACKAGE_H


#include "../../soundtouch/include/SoundTouch.h"
#include "../common/AudioProperty.h"
#include "AudioFilter.h"
#include <stdint.h>

class FilterPackage {

private:
    soundtouch::SoundTouch *pSoundTouch{nullptr};
    AudioFilter *pAudioFilter{nullptr};
    AudioProperty outProperty;
    double volume = 1;
    int filter = EFFECT_PRESET_NONE;


public:

    FilterPackage(AudioProperty outProperty);

    size_t process(void *data, int32_t size);

    void setPitch(int pitch);

    void setVolume(double volume);

    void setFilter(int type);

    virtual ~FilterPackage();
};


#endif //SUPERPLAYER_FILTERPACKAGE_H
