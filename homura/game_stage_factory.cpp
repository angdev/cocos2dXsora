// Ŭnicode please
#include "stdafx.h"
#include "game_object_factory.h"
#include "game_stage.h"
#include "game_stage_factory.h"

GameStageFactory::GameStageFactory(GameWorld *world) : world_(world) {

}

GameStageFactory::~GameStageFactory() {

}

GameStage *GameStageFactory::CreateTestStage() {
    GameStage *stage = new GameStage(world_);

    

    return stage;
}