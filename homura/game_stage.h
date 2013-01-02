// Ŭnicode please
#ifndef __GAME_STAGE_H__
#define __GAME_STAGE_H__

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class GameWorld;
class GameTriggerHandler;
typedef std::shared_ptr<GameTriggerHandler> GameTriggerHandlerPtr;

class CCParallaxScrollNode;

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
    bool IsCleared() { return is_cleared_; }
    void set_is_cleared(bool is_cleared) { is_cleared_ = is_cleared; }
    bool IsGameOver() { return is_game_over_; }
    

public:
    cocos2d::CCLayer *layer() { return layer_; }
    GameWorld *world() { return world_; }

private:
    GameWorld *world_;
    //게임 객체 추가되는 레이어
    cocos2d::CCLayer *layer_;
    //움직이는 배경
    
    CCParallaxScrollNode *parallax_;
    GameObjectFactory *factory_;
    
    float elapsed_time_;

    bool is_cleared_;
    bool is_game_over_;
};

#endif