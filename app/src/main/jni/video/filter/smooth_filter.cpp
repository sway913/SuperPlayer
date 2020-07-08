//
// Created by Lvlingling on 2020/6/30.
//

#include "smooth_filter.h"

SmoothFilter::SmoothFilter() : BaseFilter(VERREX_SFADER, SMOOTH_FRAG) {}

void SmoothFilter::onInit() {
    glUniformLevel = glGetUniformLocation(programId, "intensity");
    glUniformsample = glGetUniformLocation(programId, "singleStepOffset");
}


void SmoothFilter::preDraw() {
    glUniform1f(glUniformLevel, smoothLevel);
    GLfloat s[] = {1.0f / width, 1.0f / height};
    glUniform2fv(glUniformsample, 1, s);
}

void SmoothFilter::setSmoothLevel(float l) {
    this->smoothLevel = l;
}