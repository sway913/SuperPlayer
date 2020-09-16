//
// Created by ldr on 2020/9/16.
//

#ifndef SUPERPLAYER_THREAD_SAFE_QUEUE_H
#define SUPERPLAYER_THREAD_SAFE_QUEUE_H

#include <mutex>
#include <queue>

template<typename T>
class BlockQueue {

private:

    mutable std::mutex mutex;
    std::queue<T> data_queue;

public:

    void push(T data) {
        std::lock_guard<std::mutex> lock(mutex);
        data_queue.push(data);
    }

    bool pop(T &value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (data_queue.empty()) {
            return false;
        }
        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return data_queue.empty();
    }
};

#endif //SUPERPLAYER_THREAD_SAFE_QUEUE_H
