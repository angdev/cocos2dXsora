﻿// Ŭnicode please
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
class GameEventHandler;

typedef unsigned int EventID;
typedef std::vector<EventID> NextEvents;
typedef std::shared_ptr<NextEvents> NextEventsPtr;
typedef std::shared_ptr<GameEventHandler> GameEventHandlerPtr;

class GameObjectFactory {
public:
    explicit GameObjectFactory(GameWorld *world) { world_ = world; }
    
public:
    //TODO
    //나중에 Create 오버로딩할 예정 (구조체 헤더 이용)
    GameObject *Create(const TestPlayerObjectHeader &header, cocos2d::CCNode *parent);
    GameObject *Create(const TestBulletObjectHeader &header, cocos2d::CCNode *parent);
    GameObject *Create(const TestCombatPlaneObjectHeader &header, cocos2d::CCNode *parent);
    GameObject *Create(const DemoObjectHeader &header, cocos2d::CCNode *parent);

    //가상의 게임 객체들 (로직)
    GameObject *Create(const GameEventObjectHeader &header, EventID event_id, NextEventsPtr next_events, 
        GameEventHandlerPtr game_event_handler_);

private:
    b2Body *CreateCollisionBox(const glm::vec2 &ut_pos, float half_width, float half_height);
    GameWorld *world_;
};

#endif