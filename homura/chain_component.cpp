// Ŭnicode please
#include "stdafx.h"
#include "chain_component.h"

ChainComponent::ChainComponent(GameObject *obj) : LogicComponent(obj) {

}

ChainComponent::~ChainComponent() {

}

CompType ChainComponent::type() const {
    return kCompChain;
}

void ChainComponent::Update(float dt) {
    
}

void ChainComponent::InitMsgHandler() {

}
