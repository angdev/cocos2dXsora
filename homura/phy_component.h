// Ŭnicode please
#ifndef __PHY_COMPONENT_H__
#define __PHY_COMPONENT_H__

#include "game_component.h"

#if SR_USE_PCH == 0
#include <Box2D/Box2D.h>
#endif

class SinglePhyComponent;

struct PhyBodyInfo {
    PhyBodyInfo() : x(0), y(0), angle_rad(0) {}

    float x;
    float y;
    float angle_rad;
};

class PhyComponent : public GameComponent {
public:
    virtual b2Body *main_body() = 0;
    virtual void set_main_body(b2Body *body) = 0;

    virtual ~PhyComponent() {}
protected:
    PhyComponent(GameObject *obj) : GameComponent(obj) { }

    //실제 객체 생성에서 주로 사용할것
public:
    static SinglePhyComponent *SinglePhy(GameObject *obj, b2Body *body);
};

class SinglePhyComponent : public PhyComponent {
public:
    SinglePhyComponent(GameObject *obj, b2Body *body);
    virtual ~SinglePhyComponent();

public:
    virtual void Update(float dt);
    virtual void InitMsgHandler();

    //메시지 함수들
public:
    //2개 묶어서 SetPhyBodyInfo로 해도 될듯
    void OnMoveToMessage(MoveToMessage *msg);
    void OnMoveByMessage(MoveByMessage *msg);
    void OnSetAngleMessage(SetAngleMessage *msg);
    void OnRequestPhyBodyInfoMessage(RequestPhyBodyInfoMessage *msg);
    void OnSetPhyBodyInfoMessage(SetPhyBodyInfoMessage *msg);

public:

    virtual CompType type() const { return kCompSinglePhy; }
    virtual b2Body *main_body() { return body_; }
    virtual void set_main_body(b2Body *body);
private:
    b2Body *body_;
    //이동 목표 지점
    b2Vec2 end_point_;
};

#endif