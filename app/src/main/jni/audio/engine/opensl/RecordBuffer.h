//
// Created by ldr on 2020/6/15.
//

#ifndef POKEKARA_RECORDBUFFER_H
#define POKEKARA_RECORDBUFFER_H


class RecordBuffer {

public:
    short **buffer;
    int index = -1;
public:
    RecordBuffer(int buffersize);

    ~RecordBuffer();

    /**
     * 得到一个新的录制buffer
     * @return
     */
    short *getRecordBuffer();

    /**
     * 得到当前录制buffer
     * @return
     */
    short *getNowBuffer();
};


#endif //POKEKARA_RECORDBUFFER_H
