//
// Created by ldr on 2019-12-21.
//

#ifndef SUPERPLAYER_TEST_H
#define SUPERPLAYER_TEST_H

#include <future>
#include <stdio.h>

class Test{

private:
    std::mutex mutex;
    std::condition_variable cond;
    bool looper = true;
    std::future<void> result;

public:

    void threadFun(){

        while(looper){
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait(lock,[this]{
                if(!looper){
                    LOGI("test unlocked");
                    return true;
                }
                LOGI("test locked");
                return false;
            });

            if(!looper) {
                LOGI("test exit");
                break;
            }
        }
    }

    Test() {

    }

    void start() {
        result = std::async(std::launch::async,&Test::threadFun,this);
    }

    ~Test(){}

    void stop(){
        LOGI("test stop");
        looper = false;
        cond.notify_all();
    }


};

#endif //SUPERPLAYER_TEST_H
