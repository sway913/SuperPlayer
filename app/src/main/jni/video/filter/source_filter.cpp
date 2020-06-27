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
        "\n"
        "varying vec2 textureCoordinate;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = position;\n"
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
}



void SourceFilter::bindTexture(GLuint textureId) {
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId);
}


SourceFilter::~SourceFilter() = default;
