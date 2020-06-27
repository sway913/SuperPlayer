//
// Created by Lvlingling on 2020/6/27.
//

#ifndef SUPERPLAYER_SOURCE_H
#define SUPERPLAYER_SOURCE_H

#include <GLES2/gl2.h>

class Source {

public:

    virtual void open(int width, int height) = 0;

    virtual GLuint produceFrame() = 0;

    virtual void close() = 0;

    virtual ~Source() {

    }

};

#endif //SUPERPLAYER_SOURCE_H
