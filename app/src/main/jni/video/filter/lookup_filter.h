//
// Created by Lvlingling on 2020/6/30.
//

#ifndef SUPERPLAYER_LOOKUP_FILTER_H
#define SUPERPLAYER_LOOKUP_FILTER_H


#include "base_filter.h"
#include "../utils/png_decoder.h"

static const char *LOOK_FRAG = "varying highp vec2 textureCoordinate;\n"
                               "\n"
                               "uniform sampler2D inputImageTexture;\n"
                               "uniform sampler2D lookupTexture;\n"
                               "\n"
                               "uniform lowp float intensity;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    float lookupDimension = 64.0;\n"
                               "    highp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
                               "\n"
                               "    highp float blueColor = textureColor.b * (lookupDimension - 1.0);\n"
                               "    highp float lookupDimensionSqrt = sqrt(lookupDimension);\n"
                               "\n"
                               "    highp vec2 quad1;\n"
                               "    quad1.y = floor(floor(blueColor) / lookupDimensionSqrt);\n"
                               "    quad1.x = floor(blueColor) - (quad1.y * lookupDimensionSqrt);\n"
                               "\n"
                               "    highp vec2 quad2;\n"
                               "    quad2.y = floor(ceil(blueColor) / lookupDimensionSqrt);\n"
                               "    quad2.x = ceil(blueColor) - (quad2.y * lookupDimensionSqrt);\n"
                               "\n"
                               "    highp vec2 texPos1;\n"
                               "    texPos1.x = (quad1.x / lookupDimensionSqrt) + 0.5/lookupDimensionSqrt/lookupDimension + ((1.0/lookupDimensionSqrt - 1.0/lookupDimensionSqrt/lookupDimension) * textureColor.r);\n"
                               "    texPos1.y = (quad1.y / lookupDimensionSqrt) + 0.5/lookupDimensionSqrt/lookupDimension + ((1.0/lookupDimensionSqrt - 1.0/lookupDimensionSqrt/lookupDimension) * textureColor.g);\n"
                               "\n"
                               "    highp vec2 texPos2;\n"
                               "    texPos2.x = (quad2.x / lookupDimensionSqrt) + 0.5/lookupDimensionSqrt/lookupDimension + ((1.0/lookupDimensionSqrt - 1.0/lookupDimensionSqrt/lookupDimension) * textureColor.r);\n"
                               "    texPos2.y = (quad2.y / lookupDimensionSqrt) + 0.5/lookupDimensionSqrt/lookupDimension + ((1.0/lookupDimensionSqrt - 1.0/lookupDimensionSqrt/lookupDimension) * textureColor.g);\n"
                               "\n"
                               "    lowp vec4 newColor1 = texture2D(lookupTexture, texPos1);\n"
                               "    lowp vec4 newColor2 = texture2D(lookupTexture, texPos2);\n"
                               "\n"
                               "    lowp vec4 newColor = vec4(mix(newColor1.rgb, newColor2.rgb, fract(blueColor)), textureColor.w);\n"
                               "    gl_FragColor = mix(textureColor, newColor, intensity);\n"
                               "}";

class LookupFilter : public BaseFilter {

public:

    LookupFilter();

    void onInit() override;

    void preDraw() override;

    void draw(VideoFrame *frame) override;

    void setTablePath(char *path);

    virtual ~LookupFilter();

private:

    GLuint glUniIntensity{0};
    GLuint glUniLookupTexture{0};
    float intensity{1.0};
    volatile bool isNewLookupTable{false};
    PngDecoder *pngDecoder{nullptr};
    RGBAFrame *rgbaFrame{nullptr};
    bool isValid{false};

};


#endif //SUPERPLAYER_LOOKUP_FILTER_H
