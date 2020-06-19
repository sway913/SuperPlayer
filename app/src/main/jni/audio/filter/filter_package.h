//
// Created by ldr on 2020/6/17.
//

#ifndef SUPERPLAYER_FILTER_PACKAGE_H
#define SUPERPLAYER_FILTER_PACKAGE_H

#include <list>
#include "ifilter.h"
#include "volume_filer.h"
#include "pitch_filter.h"
#include "ff_filter.h"
#include "custom_filter.h"
#include <mutex>

using namespace std;

enum Tracker {
    Vocal,
    Acc
};

class FilterPackage {

    enum FilterType {
        None,
        FF_Filter,
        Custom
    };

private:

    list<IFilter *> filters{};
    mutex _mutex;
    FilterType filter_type{None};

public:

    void init(Tracker tracker, int sample_rate, int channels = 2);

    int process(short *data, int len);

    void setVolume(float vol);

    void setPitch(float pitch);

    void setEffect(int type);

    void setCustomEffect(float *arr);

    void destroy();

};


#endif //SUPERPLAYER_FILTER_PACKAGE_H
