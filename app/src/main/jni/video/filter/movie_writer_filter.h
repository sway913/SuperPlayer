//
// Created by Lvlingling on 2020/6/26.
//

#ifndef SUPERPLAYER_MOVIE_WRITER_FILTER_H
#define SUPERPLAYER_MOVIE_WRITER_FILTER_H

#include <atomic>
#include "base_filter.h"
#include "../encoder/VideoEncoder.h"
#include "../opengl/egl_core.h"
#include <android/native_window.h>
#include <EGL/egl.h>

class MovieWriterFilter : public BaseFilter {

public:

    MovieWriterFilter();

    void bindFrameBuffer(int w, int h) override;

    GLuint draw(GLuint textureId, int w, int h) override;

    void startRecord(JNIEnv *env, const char *video_path);

    void stopRecord();

    void setState(bool isPause);

    virtual ~MovieWriterFilter();

private:

    void releaseRecord();

    VideoEncoder *videoEncoder{nullptr};
    EGLSurface eglSurface{nullptr};
    EglCore *eglCore{nullptr};
    volatile std::atomic_bool is_recording{false};
    volatile std::atomic_bool stop_record{false};

};


#endif //SUPERPLAYER_MOVIE_WRITER_FILTER_H
