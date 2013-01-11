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
    //여기서 시작 메시지
    void Start();

public:
    bool IsCleared() { return is_cleared_; }
    void set_is_cleared(bool is_cleared) { is_cleared_ = is_cleared; }
    bool IsGameOver() { return is_game_over_; }
    CCParallaxScrollNode *parallax() { return parallax_; }

public:
    cocos2d::CCLayer *layer() { return layer_; }
    GameWorld *world() { return world_; }
    cocos2d::CCSpriteBatchNode *bullet0_node() { return bullet0_node_; }
    
    template <typename T>
    T& MakeObjectHeader(T &header, float x, float y, float start_x, float start_y) {
        header.x = x;
        header.y = y;
        header.start_x = start_x;
        header.start_y = start_y;
        return header;
    }

    template <typename T>
    GameObject *MakeCreateObjectTriggerObject(const T &header, float timer, int current_id, bool with_ally = false) {
        static AllyPlaneObjectHeader ally_header;
        GameTriggerHandler *trg_hnd = new GameTriggerHandler();
        GameTrigger *trg = new GameTrigger(this);
        GameAction *act;
        if(with_ally) {
            ally_header.x = header.x;
            ally_header.y = header.y+300;
            ally_header.start_x = header.start_x;
            ally_header.start_y = header.start_y+300;
            act = MakeCreateObjectsWithChainAction(header, ally_header);
        }
        else {
            act = MakeCreateObjectAction(header);
        }
        trg->set_action(act);
        trg->set_condition(new TimerCondition(timer));
        trg_hnd->AddTrigger(trg);
        GameTriggerObjectHeader trg_header;
        NextTriggers *next_trgs = new NextTriggers();
        next_trgs->push_back(current_id+1);
        return factory_->Create(trg_header, current_id, next_trgs, GameTriggerHandlerPtr(trg_hnd));
    }

private:
    GameWorld *world_;
    //게임 객체 추가되는 레이어
    cocos2d::CCLayer *layer_;
    //움직이는 배경
    
    CCParallaxScrollNode *parallax_;

    //그리기 최적화
    cocos2d::CCSpriteBatchNode *bullet0_node_;;


    GameObjectFactory *factory_;
    
    float elapsed_time_;

    bool is_cleared_;
    bool is_game_over_;
};

#endif
