
#ifndef SUPERPLAYER_OBSERVER_H
#define SUPERPLAYER_OBSERVER_H


class Observer {

public:
    virtual void onEngineReady() = 0;

    virtual void onError() = 0;

    virtual void onCompleted() = 0;
};

#endif //SUPERPLAYER_OBSERVER_H
