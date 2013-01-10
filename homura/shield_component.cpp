// Ŭnicode please
#include "stdafx.h"
#include "shield_component.h"
#include "game_object.h"
#include "game_world.h"
#include "phy_component.h"
#include "shield_layer.h"

#include "sora/unit.h"

using namespace sora;

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

    if(!obj()->IsEnabled()) {
        return;
    }

    //지속 시간 체크
    elapsed_time_ += dt;
    if(elapsed_time_ > duration_) {
        Destroy();
        return;
    }

    //쉴드는 타겟을 따라감
    GameObjectPtr target = obj()->world()->FindObject(target_id_);
    PhyBodyInfo target_body_info;
    RequestPhyBodyInfoMessage body_info_msg = RequestPhyBodyInfoMessage::Create(&target_body_info);
    if(target != NULL) {
        target->OnMessage(&body_info_msg);
        assert(body_info_msg.is_ret);
        //리턴값이 없으면 이상한거
        SetPhyBodyInfoMessage set_body_info_msg = SetPhyBodyInfoMessage::Create(&target_body_info);
        obj()->OnMessage(&set_body_info_msg);
    }
    //타겟을 못찾으면 쉴드 없앰
    else {
        Destroy();
    }

    //그리기
    
    obj()->world()->shield_layer->RequestRenderShield(obj()->id(),
        Unit::ToUnitFromMeter(obj()->phy_comp()->main_body()->GetPosition()));
        
}

void ShieldComponent::AfterDestroy() {
    GameObjectPtr target = obj()->world()->FindObject(target_id_);
    if(target != NULL) {
        static_cast<CharacterComponent*>(target->logic_comp())->set_unbeatable(false);
    }
    //obj()->world()->shield_layer->StopRenderShield(obj()->id());
}
