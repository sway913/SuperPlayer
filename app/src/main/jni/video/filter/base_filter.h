//
// Created by Lvlingling on 2020/6/26.
//

#ifndef SUPERPLAYER_BASE_FILTER_H
#define SUPERPLAYER_BASE_FILTER_H


#include <GLES2/gl2.h>
#include "v_filter.h"

const char VERREX_SFADER[] = "attribute vec4 position;\n"
                             "attribute vec4 inputTextureCoordinate;\n"
                             "\n"
                             "varying vec2 textureCoordinate;\n"
                             "\n"
                             "void main()\n"
                             "{\n"
                             "    gl_Position = position;\n"
                             "    textureCoordinate = inputTextureCoordinate.xy;\n"
                             "}";

const char FRAGMENT_SHADER[] = "varying highp vec2 textureCoordinate;\n"
                               "\n"
                               "uniform sampler2D inputImageTexture;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n"
                               "}";

class BaseFilter : public VFilter {

public:

    BaseFilter();

    BaseFilter(const char *vertexShader, const char *fragShader);

    virtual GLuint draw(GLuint textureId, int w, int h) override;

    virtual void destroy() override;

protected:

    virtual void onInit();

    virtual void bindFrameBuffer(int w, int h);

    virtual void bindTexture(GLuint textureId);

    virtual void preDraw();

    GLuint programId{0};
    GLuint glAttrPosition{0};
    GLuint glAttrTextureCoordinate{0};
    GLuint glUniformTexture{0};
    int width{0};
    int height{0};
    GLuint frame_buffer{0};
    GLuint texture_id{0};

private:

    void init();

    bool initialized{false};

    const char *vertex_shader{nullptr};
    const char *frag_shader{nullptr};


};


#endif //SUPERPLAYER_BASE_FILTER_H
