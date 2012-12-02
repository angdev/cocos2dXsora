// Ŭnicode please
#include "stdafx.h"
#include "drawable_component.h"
#include "phy_component.h"
#include "game_object.h"
#include "sora/unit.h"

USING_NS_CC;
using namespace std;
using namespace sora;

DrawableComponent::DrawableComponent(GameObject *obj, cocos2d::CCNode *parent)
: GameComponent(obj), 
parent_(parent) {
}

NodeDrawableComponent::NodeDrawableComponent(GameObject *obj, cocos2d::CCNode *parent, cocos2d::CCNode *node)
: DrawableComponent(obj, parent), 
node_(node) {
    parent->addChild(node);
}

NodeDrawableComponent::~NodeDrawableComponent() {
    if(node_ != NULL) {
        node_->removeFromParentAndCleanup(true);
        node_ = NULL;
    }
}

void NodeDrawableComponent::Update(float dt) {
    if(node_ == NULL) {
        return;
    }
    if(node_->isVisible() == false) {
        return;
    }
    PhyComponent *phy = this->obj()->phy_comp();
    if(phy == NULL) {
        return;
    }
    b2Body *body = phy->main_body();
    float angle_rad = body->GetAngle();
    const b2Vec2 &mt_pos = body->GetPosition();
    glm::vec2 ut_pos = Unit::ToUnitFromMeter(mt_pos);
    float angle_deg = kmRadiansToDegrees(angle_rad);
    node_->setPosition(ccp(ut_pos.x, ut_pos.y));
    node_->setRotation(-angle_deg);
}

void NodeDrawableComponent::InitMsgHandler() {
}