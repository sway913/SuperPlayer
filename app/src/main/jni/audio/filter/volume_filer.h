//
// Created by ldr on 2020/6/17.
//

#ifndef SUPERPLAYER_VOLUME_FILER_H
#define SUPERPLAYER_VOLUME_FILER_H


#include "ifilter.h"
#include "../../common/common_tools.h"

class VolumeFilter : public IFilter {

private:

    float volume = 1.0f;

public:

    void init(int sampleRate, int channel, int param) override final;

    int process(short *data, int len) override final;

    void destroy() override final;

    void setVolume(float vol);

    FilterType getType() override final ;
};


#endif //SUPERPLAYER_VOLUME_FILER_H
