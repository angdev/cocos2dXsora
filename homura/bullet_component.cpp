// Ŭnicode please
#include "stdafx.h"
#include "bullet_component.h"
#include "game_object.h"
#include "phy_component.h"
#include "data_define.h"
#include "sora/unit.h"
#include "game_world.h"

using namespace sora;

BulletComponent::BulletComponent(GameObject *obj)
    : LogicComponent(obj), speed_(0), damage_(0), from_enemy_(false) {

}


BulletComponent::~BulletComponent() {

}

void BulletComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &BulletComponent::OnBulletDamageObjectMessage);
    RegisterMsgFunc(this, &BulletComponent::OnOutOfBoundMessage);
}

void BulletComponent::Update(float dt) {
    //TODO
    //일단 방향 정해진대로 날아가도록만 하였다
    PhyBodyInfo body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&body_info);
    obj()->OnMessage(&body_info_msg);

    if(!body_info_msg.is_ret)
        return;

    b2Vec2 velocity_vec(glm::cos(body_info.angle_rad), glm::sin(body_info.angle_rad));
    velocity_vec *= speed_;
    MoveByMessage move_msg = MoveByMessage::Create(Unit::ToUnitFromMeter(velocity_vec), 0.016);
    obj()->OnMessage(&move_msg);
}

void BulletComponent::OnBulletDamageObjectMessage(BulletDamageObjectMessage *msg) {
    
    CollideBulletMessage apply_msg = CollideBulletMessage::Create(obj(), damage_, from_enemy_);
    msg->obj->OnMessage(&apply_msg);

    //리턴값. 맞았는가에 대한 체크
    if(apply_msg.applied) {
        Destroy();
    }

}

void BulletComponent::Destroy()
{
    GameWorld *world = obj()->world();
    world->RequestRemoveObject(world->FindObject(obj()->id()));

    //메시지를 월드에 뿌리는 이유는 월드에서 특정 객체가 파기되는 것을 체크하는 객체가 있을 수 있으므로
    //다 이렇게 바꿔야 함
    DestroyMessage msg = DestroyMessage::Create(obj()->id());
    world->OnMessage(&msg);
}

void BulletComponent::OnOutOfBoundMessage(OutOfBoundMessage *msg) {
    //밖에 나가면 없애야지
    Destroy();
}
