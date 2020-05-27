//
// Created by Lvlingling on 2019-11-04.
//

#include "PlayerEngine.h"
#include "../../stream/MusicStream.h"


PlayerEngine::PlayerEngine() : BaseEngine(false) {

}


IStream *PlayerEngine::setupStream() {
    MusicStream *musicStream =  new MusicStream();
    musicStream->setOutputStream(outputStream);
    return musicStream;
}

PlayerEngine::~PlayerEngine() = default;


