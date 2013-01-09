// Ŭnicode please
#ifndef __GAME_OBJECT_FACTORY_H__
#define __GAME_OBJECT_FACTORY_H__

#include "game_object.h"
#include "data_define.h"

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#include "cocos2d.h"
#endif

class b2Body;
class GameObject;
class GameWorld;
class GameTriggerHandler;

typedef unsigned int TriggerID;
typedef std::vector<TriggerID> NextTriggers;
typedef std::shared_ptr<NextTriggers> NextTriggersPtr;
typedef std::shared_ptr<GameTriggerHandler> GameTriggerHandlerPtr;

class GameObjectFactory {
public:
    explicit GameObjectFactory(GameWorld *world) { world_ = world; }
    
public:
    //TODO
    //나중에 Create 오버로딩할 예정 (구조체 헤더 이용)
    GameObject *Create(const PlayerObjectHeader &header, cocos2d::CCNode *parent);
    GameObject *Create(const BulletObjectHeader &header, cocos2d::CCNode *parent);

    //마음대로 설정 바꿔서 만들 수 있는 놈들
    GameObject *Create(const CombatPlaneObjectHeader &header, cocos2d::CCNode *parent);
    GameObject *Create(const LaserPlaneObjectHeader &header, cocos2d::CCNode *parent);

    //Preset이 있는 놈들
    GameObject *Create(const CruiserPlaneObjectHeader &header, cocos2d::CCNode *parent);
    GameObject *Create(const DeadstarPlaneObjectHeader &header, cocos2d::CCNode *parent);
    

    //테스트용
    GameObject *Create(const ObjectHeader &header, cocos2d::CCNode *parent);

    //게임 로직 보조
    GameObject *Create(const ShieldHeader &header, cocos2d::CCNode *parent);
    GameObject *Create(const FormationHeader &header);
    GameObject *Create(const ChainHeader &header);
    
    
    GameObject *Create(const GameTriggerObjectHeader &header, TriggerID trigger_id, NextTriggers *next_triggers, 
        GameTriggerHandlerPtr game_trigger_handler_);

private:
    b2Body *CreateCollisionCircle(const glm::vec2 &ut_pos, float radius);
    GameWorld *world_;
};

#endif