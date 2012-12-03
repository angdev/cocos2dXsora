// Ŭnicode please
#ifndef __CHARACTER_COMPONENT_H__
#define __CHARACTER_COMPONENT_H__

#include "logic_component.h"
#include "game_globals.h"

//캐릭터가 가지는 수치를 header가지고 만들어줄 수 있게 하려고 함.
class CharacterComponent : public LogicComponent {
public:
    CharacterComponent(GameObject *obj);
    virtual ~CharacterComponent();

public:
    //데미지 처리
    //TODO
    //Need implementation
    void ApplyDamage(float damage);

private:

};

//AI를 가지는 캐릭터의 로직 - 적 기체나 아군 기체가 가짐.
class AICharacterComponent : public CharacterComponent {
public:
    AICharacterComponent(GameObject *obj);
    virtual ~AICharacterComponent();

public:
    //TODO
    virtual CompType type() const { return kCompAI; }
    //일단 그냥 돌아다니게 한다 - AI 작성은 어떻게 할 것인가?
	virtual void Update(float dt);
    virtual void InitMsgHandler() { }

private:
    void AIUpdate(float dt);
};


#endif