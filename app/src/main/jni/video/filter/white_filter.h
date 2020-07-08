//
// Created by Lvlingling on 2020/6/30.
//

#ifndef SUPERPLAYER_WHITE_FILTER_H
#define SUPERPLAYER_WHITE_FILTER_H


#include "base_filter.h"

static const char *WHITE_FRAG = "precision mediump float;\n"
                   "uniform sampler2D inputImageTexture;\n"
                   "uniform float level;\n"
                   "varying vec2 textureCoordinate;\n"
                   "\n"
                   "void modifyColor(vec4 color){\n"
                   "    color.r=max(min(color.r, 1.0), 0.0);\n"
                   "    color.g=max(min(color.g, 1.0), 0.0);\n"
                   "    color.b=max(min(color.b, 1.0), 0.0);\n"
                   "    color.a=max(min(color.a, 1.0), 0.0);\n"
                   "}\n"
                   "\n"
                   "void main() {\n"
                   "    vec4 nColor = texture2D(inputImageTexture, textureCoordinate);\n"
                   "    vec4 deltaColor = nColor+vec4(level * 0.15, level * 0.25, level * 0.25, 0.0);\n"
                   "    modifyColor(deltaColor);\n"
                   "    gl_FragColor = deltaColor;\n"
                   "}";

class WhiteFilter : public BaseFilter {

public:

    WhiteFilter();

    void onInit() override;

    void preDraw() override;

    void setWhiteLevel(float w);

private:

    GLuint glWhiteLevel{0};

    float white_level{0};

};


#endif //SUPERPLAYER_WHITE_FILTER_H
