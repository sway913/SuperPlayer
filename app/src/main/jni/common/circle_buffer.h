//
// Created by Lvlingling on 2020/6/7.
//

#ifndef SUPERPLAYER_CIRCLE_BUFFER_H
#define SUPERPLAYER_CIRCLE_BUFFER_H

#include <cstdint>
#include <cstring>
#include "common_tools.h"

#define MAX_READER_COUNT 4

template<class T>
class CircleBuffer {

public:
    /**
     * 初始化一个循环buffer
     * @param capacity buffer容量
     */
    explicit CircleBuffer(int capacity){
        this->size = capacity;
        buffer = new T[size];
    }

    /**
     * 销毁循环buffer
     */
    ~CircleBuffer(){
        if(buffer != nullptr){
            size = 0;
            writeIndex = 0;
            delete [] buffer;
        }
        buffer = nullptr;
    }

    /**
     * 重置
     */
    void reset(){
//        readIndex = 0;
        memset(readIndexes, 0, sizeof(int) * MAX_READER_COUNT);
        memset(readTotal, 0, sizeof(int) * MAX_READER_COUNT);
        writeIndex = 0;
        writeTotal = 0;
        nextReadKey = 0;
    }

    /**
     * 获取一个readKey，用来标识读取者是谁，读取数据时使用
     * @return
     */
    int getReadKey(){
        if(nextReadKey < MAX_READER_COUNT){
            return nextReadKey++;
        }
        return -1;
    }

    /**
     * 写入数据
     *  buffer写满后会返回起始位置继续写
     * @param data 待写入的数据
     * @param count 数量
     */
    void write(T * data, int count){
        int copied = 0;
        int copyCount = 0;
        // 这里用while的原因是，count可能比size大好几倍
        while(copied < count) {
            copyCount = MIN(size - writeIndex, count - copied);
            memcpy(buffer + writeIndex, data + copied, sizeof(T) * copyCount);
            copied += copyCount;
            writeIndex = (writeIndex + copyCount) % size;
        }
        writeTotal += count;
    }

    /**
    * 读取buffer中的数据
    * @param data 输出参数，函数将把读取到的数据填充到这个指针里
    * @param count 期望读取到的数量
    * @return 实际读取到的数量
    */
    int read(int readKey, T * data, int count){
        if(readKey < 0 || readKey >= MAX_READER_COUNT){
            return 0;
        }
        int ri = readIndexes[readKey];
        // writeIndex may change by write thread
        int wi = writeIndex;
        int readAble = wi > ri ? wi - ri : wi + size - ri;
        count = MIN(count , readAble);
        count = MIN(count , writeTotal - readTotal[readKey]);
        int copied = 0;
        int copyCount = 0;
        while(copied < count){
            copyCount = wi > ri ? wi - ri : size - ri;
            copyCount = MIN(count - copied, copyCount);
            memcpy(data + copied, buffer + ri, sizeof(T) * copyCount);
            copied += copyCount;
            ri = (ri + copyCount) % size;
        }
        readIndexes[readKey] = ri;
        readTotal[readKey] = readTotal[readKey] + copied;
        return copied;
    }

    /**
     * 当前可读取的元素个数
     * @return 元素个数
     */
    int length(int readKey){
        if(readKey < 0 || readKey >= MAX_READER_COUNT){
            return 0;
        }
        int readIndex = readIndexes[readKey];
        // writeIndex may change by write thread
        int wi = writeIndex;
        if(wi >= readIndex){
            return wi - readIndex;
        }else{
            return wi + size - readIndex;
        }
    }

    int getCapacity(){
        return size;
    }

private:
    T *buffer = nullptr;
    int size = 0;
    int readIndexes[MAX_READER_COUNT] = {0};
    int readTotal[MAX_READER_COUNT] = {0};
    int nextReadKey = 0;
    int writeIndex = 0;
    int writeTotal = 0;
};

#endif //SUPERPLAYER_CIRCLE_BUFFER_H
