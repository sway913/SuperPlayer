//
// Created by ldr on 2020/6/17.
//

#ifndef SUPERPLAYER_VOLUME_FILER_H
#define SUPERPLAYER_VOLUME_FILER_H


#include "a_filter.h"
#include "../../common/common_tools.h"

class VolumeFilter : public AFilter {

private:

    float volume = 1.0f;

public:

    void init(int sampleRate, int channel) override final;

    int process(short *data, int len) override final;

    void destroy() override final;

    void setVolume(float vol);

};


#endif //SUPERPLAYER_VOLUME_FILER_H
