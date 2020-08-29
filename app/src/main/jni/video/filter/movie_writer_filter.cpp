//
// Created by Lvlingling on 2020/6/26.
//

#include "movie_writer_filter.h"
#include "../../common/common_tools.h"
#include "../../common/android_log.h"
#include "../encoder/MediaCodecEncoder.h"
#include "../opengl/opengl_utils.h"
#include <EGL/egl.h>

MovieWriterFilter::MovieWriterFilter() : BaseFilter() {

}

void MovieWriterFilter::draw(VideoFrame *frame) {
    BaseFilter::draw(frame);
    if (!stop_record && is_recording && videoEncoder) {
        EGLDisplay mEGLDisplay = eglGetCurrentDisplay();
        EGLContext mEGLContext = eglGetCurrentContext();
        EGLSurface mEGLScreenSurface = eglGetCurrentSurface(EGL_DRAW);
        // create encoder surface
        if (eglSurface == nullptr) {
            ANativeWindow *encodeWindow = ((MediaCodecEncoder *) videoEncoder)->getEncodeWindow();
            eglCore = new EglCore();
            eglCore->init(mEGLContext);
            eglSurface = eglCore->createEGLSurface(encodeWindow);
        }
        // Draw on encoder surface
        eglCore->makeCurrent(eglSurface);
        BaseFilter::draw(frame);
        // 绘制时可能已经停止录制了，所以需要判断一下
        if (is_recording && eglSurface != nullptr) {
            gettimeofday(&tv, nullptr);
            int64_t usec = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
            if(start_time ==0) {
//                start_time = usec;
                start_time = frame->timestamp;
            }
            eglCore->setPresentationTime(eglSurface, (khronos_uint64_t) (frame->timestamp - start_time));
//            eglCore->setPresentationTime(eglSurface, (khronos_uint64_t) (usec - start_time) * 1000);
            eglCore->swapBuffer(eglSurface);
            videoEncoder->encodeFrame();
        }
        // Make screen surface be current surface
        eglMakeCurrent(mEGLDisplay, mEGLScreenSurface, mEGLScreenSurface, mEGLContext);
    }
    if (stop_record) {
        releaseRecord();
    }
}

void MovieWriterFilter::destroy() {
    if (stop_record) {
        releaseRecord();
    }
}

void MovieWriterFilter::releaseRecord() {
    if (eglSurface) {
        eglCore->destroyEGLSurface(eglSurface);
        eglCore->destroy();
        DELETEOBJ(eglCore)
        eglSurface = nullptr;
    }
    if (videoEncoder) {
        videoEncoder->stop();
        DELETEOBJ(videoEncoder)
    }
}

void MovieWriterFilter::bindFrameBuffer(int w, int h) {
    this->width = w;
    this->height = h;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MovieWriterFilter::startRecord(JNIEnv *env, const char *video_path) {
    videoEncoder = VideoEncoder::getEncoder(env, video_path, width, height, true);
}

void MovieWriterFilter::stopRecord() {
    this->stop_record = true;
}

void MovieWriterFilter::setState(bool isPause) {
    this->is_recording = !isPause;
}

bool MovieWriterFilter::updateTexture() {
    return false;
}

MovieWriterFilter::~MovieWriterFilter() = default;
