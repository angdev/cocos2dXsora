// Ŭnicode please
#include "stdafx.h"
#include "formation_component.h"
#include "game_object.h"
#include "game_world.h"

#include "sora/unit.h"

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
    //일단 귀찮으니 걍 대충 움직인다

    for(auto member : member_set_) {
        GameObjectPtr obj_ptr = obj()->world()->FindObject(member);
        b2Vec2 move_vec(-1.0f, 0);
        MoveMessage move_msg = MoveMessage::Create(move_vec);
        obj_ptr->OnMessage(&move_msg);
    }
}

void FormationComponent::OnRequestJoinFormationMessage(RequestJoinFormationMessage *msg) {
    member_set_.insert(msg->id);

    if(leader_id_ == NO_LEADER) {
        leader_id_ = msg->id;
    }

    CCLOG("formation: %d joined", msg->id);
}

void FormationComponent::OnDestroyMessage(DestroyMessage *msg) {
    //누가 파기되었는지 확인하는 메시지
    auto found = member_set_.find(msg->obj_id);
    if(found != member_set_.end()) {
        member_set_.erase(found);

        //파기된게 리더인가?
        if(msg->obj_id == leader_id_) {
            //걍 앞에 있는거를 리더로 선택
            if(!member_set_.empty())
                leader_id_ = *member_set_.begin();
        }
    }
}

