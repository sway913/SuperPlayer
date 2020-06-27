//
// Created by Lvlingling on 2020/6/27.
//

#ifndef SUPERPLAYER_TEST_SOURCE_H
#define SUPERPLAYER_TEST_SOURCE_H

#include <jni.h>
#include "source.h"

class TestSource : public Source {

public:

    TestSource(JNIEnv *env);

    void open(int w, int h) override;

    void close() override;

    GLuint produceFrame() override;

private:

    bool initialized{false};

};


#endif //SUPERPLAYER_TEST_SOURCE_H
