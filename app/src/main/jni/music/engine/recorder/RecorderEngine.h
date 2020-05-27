//
// Created by ldr on 2019-11-05.
//

#ifndef SUPERPLAYER_RECORDERENGINE_H
#define SUPERPLAYER_RECORDERENGINE_H

#include "../BaseEngine.h"

using namespace oboe;

class RecorderEngine : public BaseEngine {

protected:

    IStream *setupStream() override;

public:

    RecorderEngine();

    void start() override;

    void prepare(int sample, const char *musicPath, const char *guidePath, const char *vocalPath) override;

    virtual ~RecorderEngine();
};


#endif //SUPERPLAYER_RECORDERENGINE_H
