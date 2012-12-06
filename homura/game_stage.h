// Ŭnicode please
#ifndef __GAME_STAGE_H__
#define __GAME_STAGE_H__

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class GameWorld;
class GameEventHandler;
typedef std::shared_ptr<GameEventHandler> GameEventHandlerPtr;

//Test
class GameObjectFactory;

class GameStage {
public:
    GameStage(GameWorld *world);
    virtual ~GameStage();

public:
    bool Init();
    void Update(float dt);

public:
    void AddEventHandler(GameEventHandler *event_handler);
    //void SortEvent();

public:
    cocos2d::CCLayer *layer() { return layer_; }
    GameWorld *world() { return world_; }

private:
    GameWorld *world_;
    cocos2d::CCLayer *layer_;
    
    GameObjectFactory *factory_;
    
    //스케쥴 리스트가 소팅되었는지 검사
    float elapsed_time_;
    unsigned int current_event_;
    typedef std::vector<GameEventHandlerPtr> GameEventHandlers;
    GameEventHandlers event_handlers_;
};

#endif