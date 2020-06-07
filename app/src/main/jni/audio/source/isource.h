//
// Created by Lvlingling on 2020-06-03.
//

#ifndef SUPERPLAYER_ISOURCE_H
#define SUPERPLAYER_ISOURCE_H

#include <string>
#include <future>
#include <iostream>

#include "../codec/resample_helper.h"
#include "../../common/common_tools.h"
#include "../../common/constexpr.h"

using namespace std;

class ISource {

public:

    virtual void start() = 0;

    virtual void resume() = 0;

    virtual void pause() = 0;

    virtual void stop() = 0;

    virtual bool getData(short &value) = 0;

    virtual void requestData() = 0;

    virtual bool isReady() = 0;

    virtual bool isEmpty() = 0;

    virtual void setObserver(std::function<void(long, int)>) = 0;

    virtual ~ISource() {}

protected:

    virtual void produceData() = 0;

};

#endif //SUPERPLAYER_ISOURCE_H
