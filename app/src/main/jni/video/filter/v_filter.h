//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_V_FILTER_H
#define SUPERPLAYER_V_FILTER_H

#include <GLES2/gl2.h>

class VFilter {

public:

    virtual GLuint draw(GLuint textureId, int w, int h) = 0;

    virtual void destroy() = 0;

    virtual ~VFilter() {

    }

};

#endif //SUPERPLAYER_V_FILTER_H
