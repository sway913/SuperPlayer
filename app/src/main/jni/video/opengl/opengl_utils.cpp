//
// Created by ldr on 2020/7/7.
//
#include "opengl_utils.h"


GLuint OpenGLUtils::loadShader(GLenum shaderType, const char *pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, nullptr);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char *buf = (char *) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, nullptr, buf);
                    LOGI("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
            } else {
                LOGI("Guessing at GL_INFO_LOG_LENGTH size\n");
                char *buf = (char *) malloc(0x1000);
                if (buf) {
                    glGetShaderInfoLog(shader, 0x1000, nullptr, buf);
                    LOGI("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
            }
            glDeleteShader(shader);
            shader = 0;
        }
    } else {
        LOGI("create shader error");
    }
    return shader;
}

GLuint OpenGLUtils::loadProgram(const char *pVertexSource, const char *pFragmentSource) {
    int vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        LOGI("vertexShader error");
        return 0;
    }
    int pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        LOGI("fragShader error");
        return 0;
    }
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char *buf = (char *) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, nullptr, buf);
                    LOGI("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
            LOGI("gllink error");
        }
    }
    return program;
}

GLuint OpenGLUtils::loadTexture(uint8_t *tableData, int w, int h) {
    GLuint textures;
    glGenTextures(1, &textures);
    glBindTexture(GL_TEXTURE_2D, textures);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) w, (GLsizei) h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tableData);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textures;
}

GLuint OpenGLUtils::createOEXTexture() {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, tex);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
    return tex;
}

void OpenGLUtils::createFrameBuffers(int textureWidth, int textureHeight, GLuint &frame_id, GLuint &texture_id) {
    glGenFramebuffers(1, &frame_id);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, frame_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
}

void OpenGLUtils::checkGlError(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("after %s() glError (0x%x)\n", op, error);
    }
}

void OpenGLUtils::destroyFrameBuffers(GLuint &frame_id, GLuint &texture_id) {
    if (frame_id == 0 || texture_id == 0) {
        return;
    }
    glDeleteTextures(1, &texture_id);
    glDeleteFramebuffers(1, &frame_id);
}

void OpenGLUtils::deleteTexture(GLuint texId) {
    if (texId > 0) {
        glDeleteTextures(1, &texId);
    }
}