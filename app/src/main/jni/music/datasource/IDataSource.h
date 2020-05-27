//
// Created by Lvlingling on 2019-11-06.
//

#ifndef SUPERPLAYER_IDATASOURCE_H
#define SUPERPLAYER_IDATASOURCE_H

#include <stdint.h>
#include <include/SoundTouch.h>
#include "oboe/Oboe.h"
#include "../common/Common.h"
#include "../common/LockFreeQueue.h"
#include "../common/AudioProperty.h"
#include "../filter/FilterPackage.h"

using namespace oboe;

class IDataSource {

private:

protected:

    AudioProperty outProperty;
    AudioProperty inProperty;
    STATUS status;
    FilterPackage *filterPackage{nullptr};

public:

    IDataSource(AudioProperty outProperty, AudioProperty inProperty) {
        this->outProperty = outProperty;
        this->inProperty = inProperty;
        filterPackage = new FilterPackage(outProperty);
    };

    virtual ~IDataSource() {};

    virtual void prepare() = 0;

    virtual bool getData(int16_t &value) = 0;

    virtual void requestData() = 0;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void resume() = 0;

    virtual void pause() = 0;

    virtual void seek(int64_t millis) = 0;

    virtual bool isEmpty() = 0;

    virtual bool isReady() = 0;

    virtual int64_t getTotalMs() = 0;

    virtual int64_t getCurrentMs() = 0;

    virtual void setFilter(int type) {
        filterPackage->setFilter(type);
    };

    virtual void setVolume(double volume) {
        filterPackage->setVolume(volume);
    };

    virtual void setPitch(int pitch) {
        filterPackage->setPitch(pitch);
    };

    virtual size_t processData(void *data, int32_t size) {
        return filterPackage->process(data, size);
    }


};


#endif //SUPERPLAYER_IDATASOURCE_H
