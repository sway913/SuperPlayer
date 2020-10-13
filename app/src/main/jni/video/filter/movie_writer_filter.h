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
#include <sys/time.h>

class MovieWriterFilter : public BaseFilter {

public:

    MovieWriterFilter();

    void bindFrameBuffer(int w, int h) override;

    void draw(VideoFrame *frame) override;

    void startRecord(JNIEnv *env, const char *video_path);

    void stopRecord();

    void setState(bool isPause);

    bool updateTexture() override;

    void destroy() override;

    void encodeFrame(uint8_t *frame);

    virtual ~MovieWriterFilter();

private:

    void releaseRecord();

    VideoEncoder *videoEncoder{nullptr};
    EGLSurface eglSurface{nullptr};
    EglCore *eglCore{nullptr};
    volatile std::atomic_bool is_recording{false};
    volatile std::atomic_bool stop_record{false};
    struct timeval tv;
    long start_time{0};

};


#endif //SUPERPLAYER_MOVIE_WRITER_FILTER_H
