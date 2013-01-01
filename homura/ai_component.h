// Ŭnicode please
#ifndef __AI_COMPONENT_H__
#define __AI_COMPONENT_H__

#include "game_component.h"

class AIComponent : public GameComponent {
public:
    AIComponent(GameObject *obj) : GameComponent(obj) { }
    virtual ~AIComponent() { }

public:
    virtual void InitMsgHandler();

public:
    void OnIsEnemyMessage(IsEnemyMessage *msg);
    virtual bool IsEnemy() = 0;
    
public:
    //코드 옮기면서 잠시 사용하자
    //근데 걍 계속 써도 괜찮을듯
    CharacterComponent* char_comp();

    //음.. 좋은 방법 없나..
    //각 ai 구현체마다 상태를 적절히 받을 수 있는 방법.
    //일단 enum이니까 int로 오버라이딩을..

    //ai 객체는 상속으로 해결하지 말고 state를 걍 따로 두고 해결해도 될텐데라는 생각도 들고.
    //일단 이렇게 분리하고 쓴다
    virtual void set_state(int state) = 0;

private:
    //객체 생성시 이 시작 위치를 기억했다가 적절히 화면 밖에서 등장
    //실질적인 시작 위치
    glm::vec2 start_position_;
};

#endif