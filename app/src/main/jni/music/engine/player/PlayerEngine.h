//
// Created by Lvlingling on 2019-11-04.
//

#ifndef SUPERPLAYER_PLAYERENGINE_H
#define SUPERPLAYER_PLAYERENGINE_H


#include "../BaseEngine.h"


using namespace oboe;

class PlayerEngine : public BaseEngine {

protected:

    IStream *setupStream() override;

public:

    PlayerEngine();

    ~PlayerEngine();

};


#endif //SUPERPLAYER_PLAYERENGINE_H
