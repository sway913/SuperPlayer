//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_IFILTER_H
#define SUPERPLAYER_IFILTER_H


class IFilter {

public:

    virtual GLuint draw(GLuint textureId, int w, int h) = 0;

    virtual void destroy() = 0;

    virtual ~IFilter() {

    }

};

#endif //SUPERPLAYER_IFILTER_H
