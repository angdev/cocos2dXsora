// Ŭnicode please
#include "stdafx.h"
#include "phy_component.h"

using namespace std;

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