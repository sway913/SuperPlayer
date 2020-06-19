//
// Created by ldr on 2020/6/17.
//

#ifndef SUPERPLAYER_IFILTER_H
#define SUPERPLAYER_IFILTER_H

class IFilter {

public:

    virtual void init(int sampleRate, int channel, int param) = 0;

    virtual int process(short *data, int len) = 0;

    virtual void destroy() = 0;

    virtual ~IFilter() {

    }

};

#endif //SUPERPLAYER_IFILTER_H
