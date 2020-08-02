//
// Created by Lvlingling on 2020/6/26.
//

#ifndef SUPERPLAYER_OPENGL_UTILS_H
#define SUPERPLAYER_OPENGL_UTILS_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cstdlib>
#include "../../common/android_log.h"

class OpenGLUtils {

public:

    static GLuint createOEXTexture();

    static GLuint loadShader(GLenum shaderType, const char *pSource);

    static void checkGlError(const char *op);

    static GLuint loadProgram(const char *pVertexSource, const char *pFragmentSource);

    static void createFrameBuffers(int textureWidth, int textureHeight, GLuint &frame_id, GLuint &texture_id) ;


    static void destroyFrameBuffers(GLuint &frame_id, GLuint &texture_id) ;

    static GLuint loadTexture(uint8_t *tableData, int w, int h);

    static void deleteTexture(GLuint texId) ;
};

#endif //SUPERPLAYER_OPENGL_UTILS_H
