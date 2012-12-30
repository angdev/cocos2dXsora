// Ŭnicode please
#include "stdafx.h"
#include "guided_laser_plane_component.h"

GuidedLaserPlaneComponent::GuidedLaserPlaneComponent(GameObject *obj, cocos2d::CCNode *layer) : CharacterComponent(obj, layer) {

}

GuidedLaserPlaneComponent::~GuidedLaserPlaneComponent() {

}

void GuidedLaserPlaneComponent::Update(float dt) {
    CharacterComponent::Update(dt);
}

void GuidedLaserPlaneComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
}
