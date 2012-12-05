// Ŭnicode please
#ifndef __GAME_STAGE_H__
#define __GAME_STAGE_H__

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class GameWorld;
class StageEvent;

class GameStage {
public:
    GameStage(GameWorld *world);
    virtual ~GameStage();

public:
    bool Init();
    void Update(float dt);

public:
    void AddEvent(const StageEvent &sche_obj);
    void SortEvent();

public:
    cocos2d::CCLayer *layer() { return layer_; }
    GameWorld *world() { return world_; }

private:
    GameWorld *world_;
    cocos2d::CCLayer *layer_;

    //스케쥴 리스트가 소팅되었는지 검사
    bool is_sorted_;
    float elapsed_time_;
    typedef std::vector<StageEvent> StageEventVector;
    StageEventVector stage_events_;
};

#endif