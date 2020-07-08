//
// Created by Lvlingling on 2020/6/30.
//

#ifndef SUPERPLAYER_SMOOTH_FILTER_H
#define SUPERPLAYER_SMOOTH_FILTER_H


#include "base_filter.h"


static const char *SMOOTH_FRAG = "precision mediump float;\n"
                                 "varying mediump vec2 textureCoordinate;\n"
                                 "uniform sampler2D inputImageTexture;\n"
                                 "uniform vec2 singleStepOffset;\n"
                                 "uniform float intensity;\n"
                                 "vec2 blurCoordinates[20];\n"
                                 "void main(){\n"
                                 "    blurCoordinates[0] = textureCoordinate.xy + singleStepOffset *vec2(0.0, -10.0);\n"
                                 "    blurCoordinates[1] = textureCoordinate.xy + singleStepOffset *vec2(0.0, 10.0);\n"
                                 "    blurCoordinates[2] = textureCoordinate.xy + singleStepOffset *vec2(-10.0, 0.0);\n"
                                 "    blurCoordinates[3] = textureCoordinate.xy + singleStepOffset *vec2(10.0, 0.0);\n"
                                 "    blurCoordinates[4] = textureCoordinate.xy + singleStepOffset *vec2(5.0, -8.0);\n"
                                 "    blurCoordinates[5] = textureCoordinate.xy + singleStepOffset *vec2(5.0, 8.0);\n"
                                 "    blurCoordinates[6] = textureCoordinate.xy + singleStepOffset *vec2(-5.0, 8.0);\n"
                                 "    blurCoordinates[7] = textureCoordinate.xy + singleStepOffset *vec2(-5.0, -8.0);\n"
                                 "    blurCoordinates[8] = textureCoordinate.xy + singleStepOffset *vec2(8.0, -5.0);\n"
                                 "    blurCoordinates[9] = textureCoordinate.xy + singleStepOffset *vec2(8.0, 5.0);\n"
                                 "    blurCoordinates[10] = textureCoordinate.xy + singleStepOffset *vec2(-8.0, 5.0);\n"
                                 "    blurCoordinates[11] = textureCoordinate.xy + singleStepOffset *vec2(-8.0, -5.0);\n"
                                 "    blurCoordinates[12] = textureCoordinate.xy + singleStepOffset *vec2(0.0, -6.0);\n"
                                 "    blurCoordinates[13] = textureCoordinate.xy + singleStepOffset *vec2(0.0, 6.0);\n"
                                 "    blurCoordinates[14] = textureCoordinate.xy + singleStepOffset *vec2(6.0, 0.0);\n"
                                 "    blurCoordinates[15] = textureCoordinate.xy + singleStepOffset *vec2(-6.0, 0.0);\n"
                                 "    blurCoordinates[16] = textureCoordinate.xy + singleStepOffset *vec2(-4.0, -4.0);\n"
                                 "    blurCoordinates[17] = textureCoordinate.xy + singleStepOffset *vec2(-4.0, 4.0);\n"
                                 "    blurCoordinates[18] = textureCoordinate.xy + singleStepOffset *vec2(4.0, -4.0);\n"
                                 "    blurCoordinates[19] = textureCoordinate.xy + singleStepOffset *vec2(4.0, 4.0);\n"
                                 "    vec4 currentColor = texture2D(inputImageTexture, textureCoordinate);\n"
                                 "    vec3 rgb = currentColor.rgb;\n"
                                 "    for (int i =0; i <20; i++) {\n"
                                 "        rgb += texture2D(inputImageTexture, blurCoordinates[i].xy).rgb; }\n"
                                 "    vec4 blur =vec4(rgb *1.0 /21.0, currentColor.a);\n"
                                 "    vec4 highPassColor = currentColor - blur;\n"
                                 "    highPassColor.r = clamp(2.0 * highPassColor.r * highPassColor.r *24.0, 0.0, 1.0);\n"
                                 "    highPassColor.g = clamp(2.0 * highPassColor.g * highPassColor.g *24.0, 0.0, 1.0);\n"
                                 "    highPassColor.b = clamp(2.0 * highPassColor.b * highPassColor.b *24.0, 0.0, 1.0);\n"
                                 "    vec4 highPassBlur =vec4(highPassColor.rgb, 1.0);\n"
                                 "    float b = min(currentColor.b, blur.b);\n"
                                 "    float value = clamp((b -0.2) *5.0, 0.0, 1.0);\n"
                                 "    float maxChannelColor = max(max(highPassBlur.r, highPassBlur.g), highPassBlur.b);\n"
                                 "\n"
                                 "    float currentIntensity = (1.0 - maxChannelColor / (maxChannelColor +0.2)) * value * intensity;\n"
                                 "    vec3 r = mix(currentColor.rgb, blur.rgb, currentIntensity);\n"
                                 "    gl_FragColor =vec4(r, 1.0);\n"
                                 "\n"
                                 "}";

class SmoothFilter : public BaseFilter {

public:

    SmoothFilter();

    void onInit() override;

    void preDraw() override;

    void setSmoothLevel(float l);


private:

    GLuint glUniformsample{0};
    GLuint glUniformLevel{0};
    float smoothLevel{0};

};


#endif //SUPERPLAYER_SMOOTH_FILTER_H
