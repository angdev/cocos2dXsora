// Ŭnicode please
#include "stdafx.h"
#include "character_component.h"
#include "game_object.h"
#include "phy_component.h"

//
#include <random>
#include "sora/unit.h"

CharacterComponent::CharacterComponent(GameObject *obj) 
    : LogicComponent(obj) {

}

CharacterComponent::~CharacterComponent() {

}
//CharacterComponent

AICharacterComponent::AICharacterComponent(GameObject *obj)
    : CharacterComponent(obj) {

}

AICharacterComponent::~AICharacterComponent() {

}

void AICharacterComponent::Update(float dt) {
    AIUpdate(dt);
}

void AICharacterComponent::AIUpdate(float dt) {
    GameObject *o = obj();
    if(NULL == o)
        return;
    
    PhyComponent *phy = o->phy_comp();
    if(NULL == phy)
        return;
    
    b2Body *body = phy->main_body();
    b2Vec2 body_pos = body->GetPosition();

    body_pos.y += sora::Unit::ToMeterFromUnit(1.0f);

    body->SetTransform(body_pos, body->GetAngle());
}