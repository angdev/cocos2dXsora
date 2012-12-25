// Ŭnicode please
#include "stdafx.h"
#include "formation_component.h"

FormationComponent::FormationComponent(GameObject *obj)
    : LogicComponent(obj), leader_id_(NO_LEADER) {

}

FormationComponent::~FormationComponent() {
    
}

void FormationComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &FormationComponent::OnRequestJoinFormationMessage);
    RegisterMsgFunc(this, &FormationComponent::OnDestroyMessage);
}

void FormationComponent::Update(float dt) {
    //적의 분포를 찾아서 적절히 이동하는 AI를 짜야하는데..
}

void FormationComponent::OnRequestJoinFormationMessage(RequestJoinFormationMessage *msg) {
    member_set_.insert(msg->id);

    if(leader_id_ == NO_LEADER) {
        leader_id_ = msg->id;
    }
}

void FormationComponent::OnDestroyMessage(DestroyMessage *msg) {
    //누가 파기되었는지 확인하는 메시지

}

