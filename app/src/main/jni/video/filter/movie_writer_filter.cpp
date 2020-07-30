//
// Created by Lvlingling on 2020/6/26.
//

#include "movie_writer_filter.h"
#include "../../common/common_tools.h"

MovieWriterFilter::MovieWriterFilter(JNIEnv *env) : BaseFilter() {
    videoEncoder = VideoEncoder::getEncoder(env, true);
    videoEncoder->start();
}

GLuint MovieWriterFilter::draw(GLuint textureId, int w, int h) {
    GLuint ret = BaseFilter::draw(textureId, w, h);
    if(is_recording) {
//        EGL10 mEGL = (EGL10) EGLContext.getEGL();
//        EGLDisplay mEGLDisplay = mEGL.eglGetCurrentDisplay();
//        EGLContext mEGLContext = mEGL.eglGetCurrentContext();
//        EGLSurface mEGLScreenSurface = mEGL.eglGetCurrentSurface(EGL10.EGL_DRAW);
//        // create encoder surface
//        if (mCodecInput == null) {
//            mEGLCore = new EglCore(EGL14.eglGetCurrentContext(), EglCore.FLAG_RECORDABLE);
//            mCodecInput = new WindowSurface(mEGLCore, mVideoEncoder.getSurface(), false);
//        }
//
//        // Draw on encoder surface
//        mCodecInput.makeCurrent();
//        super.draw(frame);
//        // 绘制时可能已经停止录制了，所以需要判断一下
//        if (mIsRecording && mCodecInput != null) {
//            mCodecInput.swapBuffers();
//            mVideoEncoder.frameAvailableSoon();
//        }
//        // Make screen surface be current surface
//        mEGL.eglMakeCurrent(mEGLDisplay, mEGLScreenSurface, mEGLScreenSurface, mEGLContext);
        videoEncoder->encodeFrame();
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

MovieWriterFilter::~MovieWriterFilter() {
    videoEncoder->stop();
    DELETEOBJ(videoEncoder)
}
