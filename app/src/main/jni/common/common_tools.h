//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_COMMON_TOOLS_H
#define SUPERPLAYER_COMMON_TOOLS_H

#include <future>

#define DELETEOBJ(obj) if(obj != nullptr) { delete obj; obj = nullptr; }
#define DELETEARR(arr) if(arr != nullptr) { delete[] arr; arr = nullptr; }

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))


inline void JOIN(std::future<void> &result) {
    std::future_status futureStatus;
    do {
        futureStatus = result.wait_for(std::chrono::milliseconds(20));
    } while (futureStatus != std::future_status::ready);
}


#endif //SUPERPLAYER_COMMON_TOOLS_H
