//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_SOURCE_FACTORY_H
#define SUPERPLAYER_SOURCE_FACTORY_H

#include <string>

using namespace std;

class SourceFactory {

private:

    std::shared_ptr<string> vocalPath{nullptr};
    std::shared_ptr<string> aacPath{nullptr};
    std::shared_ptr<string> guidePath{nullptr};


public:


};

#endif //SUPERPLAYER_SOURCE_FACTORY_H
