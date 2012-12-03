// Ŭnicode please
#include "stdafx.h"
#include "phy_component.h"
#include "sora/unit.h"

#if SR_USE_PCH == 0
#include "Box2D/Box2D.h"
#endif

using namespace std;
using namespace sora;

SinglePhyComponent *PhyComponent::SinglePhy(GameObject *obj, b2Body *body) {
    return new SinglePhyComponent(obj, body);
}

////////////////////////////////////////
SinglePhyComponent::SinglePhyComponent(GameObject *obj, b2Body *body)
: PhyComponent(obj),
body_(body) {
    body->SetUserData(reinterpret_cast<void*>(obj));
}

SinglePhyComponent::~SinglePhyComponent() {
    if(body_ != NULL) {
        b2World *world = body_->GetWorld();
        world->DestroyBody(body_);
        body_ = NULL;
    }
}

void SinglePhyComponent::Update(float dt) {
}

void SinglePhyComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &SinglePhyComponent::OnMoveMessage);
}

void SinglePhyComponent::OnMoveMessage(MoveMessage *msg) {
    //msg에 들어오는 값은 px임.    
    b2Vec2 vec2 = body_->GetPosition();
    vec2.x += Unit::ToMeterFromUnit(msg->vec2.x);
    vec2.y += Unit::ToMeterFromUnit(msg->vec2.y);

    body_->SetTransform(vec2, body_->GetAngle());
    body_->SetAwake(true);
}

void SinglePhyComponent::set_main_body(b2Body *body) {
    //remove prev
    if(body_ != NULL) {
        b2World *world = body_->GetWorld();
        world->DestroyBody(body_);
        body_ = NULL;
    }

    body_ = body;
    body_->SetUserData(obj());
}