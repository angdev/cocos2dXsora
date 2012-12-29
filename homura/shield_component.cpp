// Ŭnicode please
#include "stdafx.h"
#include "shield_component.h"
#include "game_object.h"
#include "game_world.h"
#include "phy_component.h"

ShieldComponent::ShieldComponent(GameObject *obj)
    : CharacterComponent(obj), elapsed_time_(0), duration_(0) {

}

ShieldComponent::~ShieldComponent() {

}

void ShieldComponent::InitMsgHandler() {
    CharacterComponent::InitMsgHandler();
}

void ShieldComponent::Update(float dt) {
    CharacterComponent::Update(dt);

    //지속 시간 체크
    elapsed_time_ += duration_;
    if(elapsed_time_ > duration_)
        Destroy();

    //쉴드는 타겟을 따라감
    GameObjectPtr target = obj()->world()->FindObject(target_id_);
    PhyBodyInfo target_body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&target_body_info);
    if(target != NULL) {
        target->OnMessage(&body_info_msg);
        assert(body_info_msg.is_ret);
        //리턴값이 없으면 이상한거
        SetPhyBodyInfoMessage set_body_info_msg = SetPhyBodyInfoMessage::Create(body_info_msg.phy_body_info);
        OnMessage(&set_body_info_msg);
    }
    //타겟을 못찾으면 쉴드 없앰
    else {
        Destroy();
    }
    
}

void ShieldComponent::AfterDestroy() {
}