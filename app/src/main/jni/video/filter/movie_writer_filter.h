//
// Created by Lvlingling on 2020/6/26.
//

#ifndef SUPERPLAYER_MOVIE_WRITER_FILTER_H
#define SUPERPLAYER_MOVIE_WRITER_FILTER_H

#include <atomic>
#include "base_filter.h"
#include "../encoder/VideoEncoder.h"

class MovieWriterFilter : public BaseFilter {

public:

    MovieWriterFilter(JNIEnv *env);

    void bindFrameBuffer(int w, int h) override;

    GLuint draw(GLuint textureId, int w, int h) override;

    void setRecord(bool isRecord);

    virtual ~MovieWriterFilter();

private:

    VideoEncoder *videoEncoder{nullptr};
    volatile std::atomic_bool is_recording{false};

};


#endif //SUPERPLAYER_MOVIE_WRITER_FILTER_H
