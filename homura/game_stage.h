// Ŭnicode please
#ifndef __GAME_STAGE_H__
#define __GAME_STAGE_H__

#include "stdafx.h"

class GameWorld;

//이 구조체에 게임 스테이지에 실행될 스케쥴을 삽입.
struct StageScheduleObject {
    //단위는 second
    float delayed_time_;
    GameObject* obj;
};

class GameStage {
public:
    GameStage(GameWorld *world);
    virtual ~GameStage();

public:
    bool Init();
    void Update(float dt) { }

public:
    cocos2d::CCLayer *layer() { return layer_; }
    GameWorld *world() { return world_; }

private:
    GameWorld *world_;
    cocos2d::CCLayer *layer_;

    std::vector<StageScheduleObject> stage_schedules_;
};

#endif