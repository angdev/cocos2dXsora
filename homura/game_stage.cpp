// Ŭnicode please
#include "stdafx.h"
#include "game_stage.h"

USING_NS_CC;

GameStage::GameStage(GameWorld *world)
    : world_(world) {

}

GameStage::~GameStage() {

}

bool GameStage::Init() {
    layer_ = CCLayer::create();

    if(NULL == layer_)
        return false;

    return true;
}