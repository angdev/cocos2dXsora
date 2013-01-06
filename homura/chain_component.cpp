// Ŭnicode please
#include "stdafx.h"
#include "chain_component.h"
#include "game_object.h"
#include "game_world.h"
#include "chain_layer.h"

#include "phy_component.h"

#include "sora/unit.h"

using namespace sora;

ChainComponent::ChainComponent(GameObject *obj) : LogicComponent(obj) {

}

ChainComponent::~ChainComponent() {

}

CompType ChainComponent::type() const {
    return kCompChain;
}

void ChainComponent::Update(float dt) {
    //테스트 움직임
    if(!obj()->IsEnabled())
        return;

    //slave는 master 와의 일정 거리를 유지하려 한다
    GameWorld *world = obj()->world();
    GameObjectPtr slave = world->FindObject(slave_id_);
    GameObjectPtr master = world->FindObject(master_id_);

    //아 귀찮다
    if(slave == NULL || master == NULL)
        return;

    b2Vec2 slave_pos = slave->phy_comp()->main_body()->GetPosition();
    b2Vec2 master_pos = master->phy_comp()->main_body()->GetPosition();
    //그리기
    obj()->world()->chain_layer->RequestDraw(obj()->id(), Unit::ToUnitFromMeter(slave_pos), Unit::ToUnitFromMeter(master_pos));

    b2Vec2 pos_diff = master_pos - slave_pos;
    
    //일정 거리 안에 있으면 그냥 리턴
    if(pos_diff.Length() < 0.5)
        return;

    //속도는 어떻게 할까나
    MoveToMessage msg = MoveToMessage::Create(Unit::ToUnitFromMeter(master_pos), 1);
    slave->OnMessage(&msg);

    return;


    /*
    b2Vec2 move_vec(0.5f, 0);
    MoveByMessage move_msg = MoveByMessage::Create(Unit::ToUnitFromMeter(move_vec), 0.1f);

    GameWorld *world = obj()->world();
    GameObjectPtr slave_ptr = world->FindObject(slave_id_);
    if(slave_ptr != NULL) {
        slave_ptr->OnMessage(&move_msg);
    }
    */
}

void ChainComponent::InitMsgHandler() {
    RegisterMsgFunc(this, &ChainComponent::OnDestroyMessage);
    RegisterMsgFunc(this, &ChainComponent::OnCheckConnectedChainMessage);
}

void ChainComponent::OnDestroyMessage(DestroyMessage *msg) {
    //마스터 혹은 슬레이브 파괴시 체인도 파괴
    if(msg->obj_id == master_id_ || msg->obj_id == slave_id_)
        Destroy();
}

void ChainComponent::OnCheckConnectedChainMessage(CheckConnectedChainMessage *msg) {
    if(msg->id == master_id_ || msg->id == slave_id_) {
        msg->checked = true;
    }
}

void ChainComponent::Destroy() {
    //disable 시키고
    obj()->ToggleEnable();

    GameWorld *world = obj()->world();
    GameObjectPtr obj_ptr = world->FindObject(obj()->id());
    world->RequestRemoveObject(obj_ptr);
    DestroyMessage msg = DestroyMessage::Create(obj()->id());
    world->OnMessage(&msg);
}
