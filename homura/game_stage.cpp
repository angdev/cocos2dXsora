// Ŭnicode please
#include "stdafx.h"
#include "game_stage.h"
#include "stage_event.h"

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

USING_NS_CC;

GameStage::GameStage(GameWorld *world)
    : world_(world), is_sorted_(0), elapsed_time_(0), layer_(0) {

}

GameStage::~GameStage() {

}

bool GameStage::Init() {
    layer_ = CCLayer::create();

    if(NULL == layer_)
        return false;

    return true;
}

void GameStage::AddEvent(const StageEvent &sche_obj) {
    stage_events_.push_back(sche_obj);
    is_sorted_ = false;
}

void GameStage::SortEvent() {
    //TODO
    //Test

    std::sort(stage_events_.begin(), stage_events_.end(), 
        [](const StageEvent &e1, const StageEvent &e2) -> bool {
            if(e1.start_time() < e2.start_time()) {
                return true;
            }
            return false;
    });

    is_sorted_ = true;
}

void GameStage::Update(float dt) {
    if(!is_sorted_)
        SortEvent();

    //스케쥴 돌면서 시간 지나면 실행
    elapsed_time_ += dt;
    
    if (!stage_events_.empty()) {
        //스테이지 스케쥴 실행
    }
}