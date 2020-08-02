//
// Created by Lvlingling on 2020/6/26.
//

#include "movie_writer_filter.h"
#include "../../common/common_tools.h"
#include "../../common/android_log.h"
#include "../encoder/MediaCodecEncoder.h"
#include <EGL/egl.h>

MovieWriterFilter::MovieWriterFilter() : BaseFilter() {

}

GLuint MovieWriterFilter::draw(GLuint textureId, int w, int h) {
    GLuint ret = BaseFilter::draw(textureId, w, h);
    if (is_recording && videoEncoder) {
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
        BaseFilter::draw(textureId, w, h);
        // 绘制时可能已经停止录制了，所以需要判断一下
        if (is_recording && eglSurface != nullptr) {
            eglCore->swapBuffer(eglSurface);
            videoEncoder->encodeFrame();
        }
        // Make screen surface be current surface
        eglMakeCurrent(mEGLDisplay, mEGLScreenSurface, mEGLScreenSurface, mEGLContext);
    } else if(eglSurface) {
        eglCore->destroyEGLSurface(eglSurface);
        eglCore->destroy();
        DELETEOBJ(eglCore)
    }
    return ret;
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
    pause();
    if (videoEncoder) {
        videoEncoder->stop();
    }
}

void MovieWriterFilter::resume() {
    this->is_recording = true;
}

void MovieWriterFilter::pause() {
    this->is_recording = false;
}

MovieWriterFilter::~MovieWriterFilter() {
    videoEncoder->stop();
    DELETEOBJ(videoEncoder)
}
