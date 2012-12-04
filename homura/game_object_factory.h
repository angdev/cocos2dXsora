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
    //TODO
    //나중에 Create 오버로딩할 예정 (구조체 헤더 이용)
    GameObject *CreateDemoBullet(const glm::vec2 &ut_pos, cocos2d::CCNode *parent);
    //사실 총알이 가질 수 있는 요소가 꽤나 많다 -> header에 다 담기는 무리일 듯하니 따로 생성함수를?
    GameObject *CreateDemoBullet(const TestBulletObjectHeader &header, cocos2d::CCNode *parent);
    GameObject *CreateDemoCombatPlane(const glm::vec2 &ut_pos, cocos2d::CCNode *parent);
    GameObject *CreateDemoEnemy(const glm::vec2 &ut_pos, cocos2d::CCNode *parent);
    GameObject *CreateDemoObj(const glm::vec2 &ut_pos, cocos2d::CCNode *parent);

    //Player 객체를 테스트 용도로 생성할 수 있게 둔다.
    GameObject *CreateDemoPlayer(const glm::vec2 &ut_pos, cocos2d::CCNode *parent);

private:
    b2Body *CreateCollisionBox(const glm::vec2 &ut_pos, float half_width, float half_height);
    GameWorld *world_;
};

#endif