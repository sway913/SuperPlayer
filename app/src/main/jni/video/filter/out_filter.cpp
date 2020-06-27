//
// Created by Lvlingling on 2020/6/26.
//

#include "out_filter.h"

OutFilter::OutFilter() : BaseFilter(VERREX_SFADER, FRAGMENT_SHADER) {}

void OutFilter::bindFrameBuffer(int w, int h) {
    this->width = w;
    this->height = h;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}