// Ŭnicode please
#include "stdafx.h"
#include "formation_component.h"
#include "game_object.h"
#include "game_world.h"
#include "phy_component.h"
#include "character_component.h"

#include "sora/unit.h"

using namespace sora;

FormationComponent::FormationComponent(GameObject *obj)
    : LogicComponent(obj), leader_id_(NO_LEADER), new_leader_(false) {

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

    //리더 기준으로 먼저 움직인다.
    if(leader_id_ == NO_LEADER) {
        return;
    }
    GameWorld *world = obj()->world();
    GameObjectPtr leader = world->FindObject(leader_id_);
    SR_ASSERT(leader != NULL && "leader error?");

    //가까운 적을 찾는다
    if(!static_cast<SinglePhyComponent*>(leader->phy_comp())->IsArrived() && new_leader_) {
        return;
    }
    new_leader_ = false;

    b2Body *leader_body = leader->phy_comp()->main_body();
    b2Vec2 leader_pos = leader_body->GetPosition();
    FindNearestEnemyMessage find_msg = FindNearestEnemyMessage::Create(leader_pos, 
        static_cast<CharacterComponent*>(leader->logic_comp())->is_enemy());
    world->OnMessage(&find_msg);

    //못 찾으면 걍 가만히 있어본다
    if(find_msg.id == -1) {
        //걍 공격
        AttackMessage msg = AttackMessage::Create(0);
        leader->OnMessage(&msg);
        return;
    }

    GameObjectPtr found_enemy = world->FindObject(find_msg.id);
    b2Vec2 enemy_pos = found_enemy->phy_comp()->main_body()->GetPosition();
    bool is_leader_attacking = false;
    bool is_leader_moving = false;

    //리더와의 거리차를 계산해보자 (x축 상의)
    float x_diff = glm::abs(enemy_pos.x - leader_pos.x);
    if(x_diff > 1.0f) {
        glm::vec2 vec(Unit::ToUnitFromMeter(enemy_pos.x), Unit::ToUnitFromMeter(leader_pos.y));
        MoveToMessage move_msg = MoveToMessage::Create(vec, 0.5f);
        leader->OnMessage(&move_msg);
        is_leader_moving = true;
    }
    else {
        //그냥 각도 돌려버린다
        /*
        MoveByMessage move_msg = MoveByMessage::Create(glm::vec2(0, 1), 1);
        leader->OnMessage(&move_msg);
        */

        SetAngleMessage ang_msg = SetAngleMessage::Create(M_PI_2);
        leader->OnMessage(&ang_msg);
        AttackMessage msg = AttackMessage::Create(0);
        leader->OnMessage(&msg);
        is_leader_attacking = true;
    }

    //나머지는 리더 주변을 따라 다니면서 적절히 발사

    for(auto member : member_set_) {
        if(member == leader_id_) {
            continue;
        }

        GameObjectPtr member_obj = world->FindObject(member);
        if(member_obj == NULL) {
            continue;
        }
        b2Body *member_body = member_obj->phy_comp()->main_body();
        b2Vec2 member_pos = member_body->GetPosition();
        b2Vec2 pos_diff = leader_pos - member_pos;
        float distance = pos_diff.Length();
        if(distance > 8.0f) {
            //리더 주변으로 오도록
            MoveByMessage move_msg = MoveByMessage::Create(Unit::ToUnitFromMeter(pos_diff), 1);
            member_obj->OnMessage(&move_msg);
        }
        else if(is_leader_attacking) {
            /*
            MoveByMessage move_msg = MoveByMessage::Create(glm::vec2(0, 0.0001), 1.0f/60);
            member_obj->OnMessage(&move_msg);
            */
            SetAngleMessage ang_msg = SetAngleMessage::Create(M_PI_2);
            member_obj->OnMessage(&ang_msg);
            AttackMessage msg = AttackMessage::Create(0);
            member_obj->OnMessage(&msg);
        }
    }
    
}

void FormationComponent::OnRequestJoinFormationMessage(RequestJoinFormationMessage *msg) {
    member_set_.insert(msg->id);

    GameObjectPtr ptr = obj()->world()->FindObject(msg->id);
    if(leader_id_ == NO_LEADER) {
        leader_id_ = msg->id;

        if(ptr != NULL) {
            MoveToMessage move_msg = MoveToMessage::Create(glm::vec2(360, 200), 1);
            ptr->OnMessage(&move_msg);
        }
        new_leader_ = true;
    }

    if(ptr != NULL) {
        static_cast<CharacterComponent*>(ptr->logic_comp())->set_unbeatable(false);
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
            if(!member_set_.empty()) {
                leader_id_ = *member_set_.begin();
            }
            else {
                leader_id_ = NO_LEADER;
                new_leader_ = true;
            }
        }
    }
}

