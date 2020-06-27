//
// Created by Lvlingling on 2020/6/27.
//

#include "test_source.h"
#include "../opengl_utils.h"


GLuint program1;
GLuint glAttrPosition1;

const char *vsCode1 = "attribute vec4 position;\n"
                      "void main()\n"
                      "{\n"
                      "gl_Position=position;\n"
                      "}\n";

const char *fsCode1 = "void main()\n"
                      "{\n"
                      "gl_FragColor=vec4(1.0,0.0,0.0,1.0);\n"
                      "}\n";
float cube1[] = {
        0.0f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
};

TestSource::TestSource(JNIEnv *env) {

}

void TestSource::open(int w, int h) {

}


GLuint TestSource::produceFrame() {
    if (!initialized) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 1, 0);
        program1 = OpenGLUtils::loadProgram(vsCode1, fsCode1);
        glAttrPosition1 = glGetAttribLocation(program1, "position");
        glUseProgram(program1);
        glEnableVertexAttribArray(glAttrPosition1);
        glVertexAttribPointer(glAttrPosition1, 2, GL_FLOAT, GL_FALSE, 0, cube1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(glAttrPosition1);
        initialized = true;
    }
    return 0;
}


void TestSource::close() {

}