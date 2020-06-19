//
// Created by ldr on 2020/6/19.
//

#ifndef SUPERPLAYER_CUSTOM_FILTER_H
#define SUPERPLAYER_CUSTOM_FILTER_H

class CustomFilter {


public:

    void init(int sampleRate, int channelCount);

    int process(short *data, int len);

    void setEffect(float *arr);

    void destroy();

private:


};


#endif //SUPERPLAYER_CUSTOM_FILTER_H
