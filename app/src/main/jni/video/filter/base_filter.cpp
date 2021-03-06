//
// Created by Lvlingling on 2020/6/26.
//

#include "base_filter.h"
#include "../opengl/opengl_utils.h"
#include <sstream>
#include <thread>


static float RECTANGLE_VERTICES[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
};

static float RECTANGLE_TEXTURE[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
};

BaseFilter::BaseFilter() {
    vertex_shader = VERREX_SFADER;
    frag_shader = FRAGMENT_SHADER;
}


BaseFilter::BaseFilter(const char *vertexShader, const char *fragShader) : vertex_shader(vertexShader), frag_shader(fragShader) {}


void BaseFilter::init() {
    if (!initialized) {
        programId = OpenGLUtils::loadProgram(vertex_shader, frag_shader);
        glAttrPosition = glGetAttribLocation(programId, "position");
        glAttrTextureCoordinate = glGetAttribLocation(programId, "inputTextureCoordinate");
        glUniformTexture = glGetUniformLocation(programId, "inputImageTexture");
        onInit();
        initialized = true;
        LOGI("load %s program id %d, positon %d, coordinate %d, texture %d", typeid(*this).name(), programId, glAttrPosition, glAttrTextureCoordinate, glUniformTexture);
    }
}

void BaseFilter::onInit() {

}

void BaseFilter::draw(VideoFrame *frame) {
    init();
    bindFrameBuffer(frame->width, frame->height);
    glUseProgram(programId);
    preDraw();
    glVertexAttribPointer(glAttrPosition, 2, GL_FLOAT, GL_FALSE, 0, RECTANGLE_VERTICES);
    glEnableVertexAttribArray(glAttrPosition);

    glVertexAttribPointer(glAttrTextureCoordinate, 2, GL_FLOAT, GL_FALSE, 0, RECTANGLE_TEXTURE);
    glEnableVertexAttribArray(glAttrTextureCoordinate);

    glActiveTexture(GL_TEXTURE0);
    bindTexture(frame->textureId);
    glUniform1i(glUniformTexture, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(glAttrPosition);
    glDisableVertexAttribArray(glAttrTextureCoordinate);
    bindTexture(0);
    if (updateTexture()) {
        frame->textureId = texture_id;
    }
}

void BaseFilter::bindFrameBuffer(int w, int h) {
    if (w != width || h != height) {
        OpenGLUtils::destroyFrameBuffers(frame_buffer, texture_id);
        OpenGLUtils::createFrameBuffers(w, h, frame_buffer, texture_id);
        this->width = w;
        this->height = h;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BaseFilter::bindTexture(GLuint textureId) {
    glBindTexture(GL_TEXTURE_2D, textureId);
}


void BaseFilter::preDraw() {

}

bool BaseFilter::updateTexture() {
    return true;
}

void BaseFilter::destroy() {
    if (initialized) {
        width = 0;
        height = 0;
        initialized = false;
        if (programId > 0) {
            glDeleteProgram(programId);
        }
        if (frame_buffer > 0) {
            OpenGLUtils::destroyFrameBuffers(frame_buffer, texture_id);
        }
    }
}