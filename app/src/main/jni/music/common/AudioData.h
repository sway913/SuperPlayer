//
// Created by ldr on 2019-11-10.
//

#ifndef SUPERPLAYER_FRAMEDATA_H
#define SUPERPLAYER_FRAMEDATA_H

class AudioData {
public:
    char *data = nullptr;
    int size = 0;

    AudioData() {

    }

    AudioData(char *data, int size) {
        this->data = new char[size];
        if (data) {
            memcpy(this->data, data, static_cast<size_t>(size));
        }
        this->size = size;
    }

    virtual ~AudioData() {
        if (!data) {
            delete data;
            data = nullptr;
        }
    }
};

#endif //SUPERPLAYER_FRAMEDATA_H
