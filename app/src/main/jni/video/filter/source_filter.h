//
// Created by Lvlingling on 2020/6/26.
//

#ifndef SUPERPLAYER_SOURCE_FILTER_H
#define SUPERPLAYER_SOURCE_FILTER_H

#include "base_filter.h"
#include <GLES2/gl2ext.h>
#include <GLES2/gl2.h>

class SourceFilter : public BaseFilter {


public:

    SourceFilter();

    void bindTexture(GLuint textureId) override;

    virtual ~SourceFilter();

};


#endif //SUPERPLAYER_SOURCE_FILTER_H
