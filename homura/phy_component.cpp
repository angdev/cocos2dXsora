// Ŭnicode please
#include "stdafx.h"
#include "phy_component.h"

using namespace std;

SinglePhyComponent *PhyComponent::SinglePhy(GameObject *obj, b2Body *body) {
    return new SinglePhyComponent(obj, body);
}

void SinglePhyComponent::Update(float dt) {
}

void SinglePhyComponent::InitMsgHandler() {
}

SinglePhyComponent::SinglePhyComponent(GameObject *obj, b2Body *body)
: PhyComponent(obj),
body_(body) {
    body->SetUserData(reinterpret_cast<void*>(obj));
}

SinglePhyComponent::~SinglePhyComponent() {

}