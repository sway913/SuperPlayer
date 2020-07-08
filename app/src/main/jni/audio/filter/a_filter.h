//
// Created by ldr on 2020/6/17.
//

#ifndef SUPERPLAYER_A_FILTER_H
#define SUPERPLAYER_A_FILTER_H

class AFilter {

public:

    virtual void init(int sampleRate, int channel) = 0;

    virtual int process(short *data, int len) = 0;

    virtual void destroy() = 0;

    virtual ~AFilter() {

    }

};

#endif //SUPERPLAYER_A_FILTER_H
