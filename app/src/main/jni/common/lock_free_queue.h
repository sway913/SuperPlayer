//
// Created by Lvlingling on 2020-06-03.
//

#ifndef SUPERPLAYER_LOCK_FREE_QUEUE_H
#define SUPERPLAYER_LOCK_FREE_QUEUE_H

#include <cstdint>
#include <atomic>
#include "android_log.h"

template <typename T, uint32_t CAPACITY, typename INDEX_TYPE = uint32_t>
class LockFreeQueue {
public:
    LockFreeQueue () {
        buffer = new T[CAPACITY];
    }

    ~LockFreeQueue() {
        LOGI("~LockFreeQueue");
        delete [] buffer;
        buffer = nullptr;
    }

    static constexpr bool isPowerOfTwo(uint32_t n) { return (n & (n - 1)) == 0; }
    static_assert(isPowerOfTwo(CAPACITY), "Capacity must be a power of 2");
    static_assert(std::is_unsigned<INDEX_TYPE>::value, "Index type must be unsigned");

    bool pop(T &val) {
        if (isEmpty()){
            return false;
        } else {
            val = buffer[mask(readCounter)];
            ++readCounter;
            return true;
        }
    }

    bool push(const T& item) {
        if (isFull()){
            return false;
        } else {
            buffer[mask(writeCounter)] = item;
            ++writeCounter;
            return true;
        }
    }

    bool peek(T &item) const {
        if (isEmpty()){
            return false;
        } else {
            item = buffer[mask(readCounter)];
            return true;
        }
    }

    INDEX_TYPE size() const {
        return writeCounter - readCounter;
    };

    void reset() {
        writeCounter = 0;
        readCounter = 0;
    }

private:

    bool isEmpty() const { return readCounter == writeCounter; }

    bool isFull() const { return size() == CAPACITY; }

    INDEX_TYPE mask(INDEX_TYPE n) const { return static_cast<INDEX_TYPE>(n & (CAPACITY - 1)); }

    T *buffer = nullptr;
    std::atomic<INDEX_TYPE> writeCounter { 0 };
    std::atomic<INDEX_TYPE> readCounter { 0 };

};

#endif //SUPERPLAYER_LOCK_FREE_QUEUE_H
