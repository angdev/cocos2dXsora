// Ŭnicode please
#ifndef __AI_CHARACTER_COMPONENT_H__
#define __AI_CHARACTER_COMPONENT_H__

#include "character_component.h"

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
    virtual void InitMsgHandler() { CharacterComponent::InitMsgHandler(); }

private:
    void AIUpdate(float dt);
};


#endif