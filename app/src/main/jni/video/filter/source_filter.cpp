//
// Created by Lvlingling on 2020/6/26.
//

#include "source_filter.h"
#include "../../common/common_tools.h"
#include "../../common/android_log.h"
#include "../opengl_utils.h"

const char gVertexShader[] =
        "attribute vec4 position;\n"
        "attribute vec4 inputTextureCoordinate;\n"
        "uniform mat4 u_Matrix;"
        "varying vec2 textureCoordinate;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = position * u_Matrix;\n"
        "    textureCoordinate = inputTextureCoordinate.xy;\n"
        "}";

const char gFragmentShader[] =
        "#extension GL_OES_EGL_image_external : require\n"
        "precision mediump float;\n"
        "uniform samplerExternalOES inputImageTexture;\n"
        "varying vec2 textureCoordinate;\n"
        "void main() {\n"
        "    gl_FragColor=texture2D(inputImageTexture, textureCoordinate);\n"
        "}";


SourceFilter::SourceFilter() : BaseFilter(gVertexShader, gFragmentShader) {
    matrix = new float[16];
}

void SourceFilter::onInit() {
    glMatrix = glGetUniformLocation(programId, "u_Matrix");
}

void SourceFilter::preDraw() {
    glUniformMatrix4fv(glMatrix, 1, GL_FALSE, (GLfloat *) matrix);
}

void SourceFilter::bindTexture(GLuint textureId) {
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId);
}

void SourceFilter::setMatirx(float *mtx) {
    memcpy(matrix, mtx, sizeof(float) * 16);
}


SourceFilter::~SourceFilter() {
    DELETEARR(matrix)
}
