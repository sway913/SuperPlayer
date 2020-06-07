
#ifndef SUPERPLAYER_OBSERVER_H
#define SUPERPLAYER_OBSERVER_H


class Observer {

public:
    virtual void onReady() = 0;

    virtual void onError() = 0;

    virtual void onCompleted() = 0;

    virtual void onProduceData(short *data, int count) = 0;
};

#endif //SUPERPLAYER_OBSERVER_H
