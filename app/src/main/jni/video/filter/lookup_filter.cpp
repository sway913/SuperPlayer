//
// Created by Lvlingling on 2020/6/30.
//

#include "lookup_filter.h"
#include "../opengl/opengl_utils.h"
#include "../../common/common_tools.h"

LookupFilter::LookupFilter() : BaseFilter(VERREX_SFADER, LOOK_FRAG) {
    pngDecoder = new PngDecoder();
}

void LookupFilter::onInit() {
    glUniIntensity = glGetUniformLocation(programId, "intensity");
    glUniLookupTexture = glGetUniformLocation(programId, "lookupTexture");
}


void LookupFilter::preDraw() {
    glUniform1f(glUniIntensity, intensity);
    if (isNewLookupTable && rgbaFrame) {
        int lookupTexture = OpenGLUtils::loadTexture(rgbaFrame->pixels, rgbaFrame->width, rgbaFrame->height);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, lookupTexture);
        glUniform1i(glUniLookupTexture, 1);
        isNewLookupTable = false;
    }
}

GLuint LookupFilter::draw(GLuint textureId, int w, int h) {
    if (isValid) {
        return BaseFilter::draw(textureId, w, h);
    } else {
        return textureId;
    }
}

void LookupFilter::setTablePath(char *path) {
    if (path) {
        DELETEOBJ(rgbaFrame)
        pngDecoder->openFile(path);
        rgbaFrame = pngDecoder->getRGBAFrame();
        pngDecoder->closeFile();
        isNewLookupTable = true;
        isValid = true;
    } else {
        isValid = false;
    }
}

LookupFilter::~LookupFilter() {
    delete pngDecoder;
    DELETEOBJ(rgbaFrame)
}


