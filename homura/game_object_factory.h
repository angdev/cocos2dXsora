// Ŭnicode please
#ifndef __GAME_OBJECT_FACTORY_H__
#define __GAME_OBJECT_FACTORY_H__

#include "game_object.h"
#include "data_define.h"

#if SR_USE_PCH == 0
#include <glm/glm.hpp>
#include "cocos2d.h"
#endif

class GameObject;
class GameWorld;

class GameObjectFactory {
public:
    explicit GameObjectFactory(GameWorld *world) { world_ = world; }

public:
    //단순 스프라이트를 가지는 게임 객체 생성
    GameObject *Create(const TestSpriteObjectHeader &header);
    //TODO
    //나중에 Create 오버로딩할 예정 (구조체 헤더 이용)
    GameObject *CreateDemoBullet(const glm::vec2 &ut_pos, cocos2d::CCNode *parent);
    GameObject *CreateDemoObj(const glm::vec2 &ut_pos, cocos2d::CCNode *parent);

private:
    GameWorld *world_;
};

#endif