//
// Created by Lvlingling on 2020/6/30.
//

#include "white_filter.h"

WhiteFilter::WhiteFilter() : BaseFilter(VERREX_SFADER, WHITE_FRAG) {}


void WhiteFilter::onInit() {
    glWhiteLevel = glGetUniformLocation(programId, "level");
}


void WhiteFilter::preDraw() {
    glUniform1f(glWhiteLevel, white_level);
}

void WhiteFilter::setWhiteLevel(float w) {
    this->white_level = w;
}