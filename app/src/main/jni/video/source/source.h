//
// Created by Lvlingling on 2020/6/27.
//

#ifndef SUPERPLAYER_SOURCE_H
#define SUPERPLAYER_SOURCE_H

#include <GLES2/gl2.h>
#include "video_frame.h"

class Source {

public:

    virtual void open(int width, int height, jobject surface = nullptr) = 0;

    virtual VideoFrame *produceFrame() = 0;

    virtual void close() = 0;

    virtual ~Source() {

    }

};

#endif //SUPERPLAYER_SOURCE_H
