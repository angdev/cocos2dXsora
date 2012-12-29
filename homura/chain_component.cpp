// Ŭnicode please
#include "stdafx.h"
#include "chain_component.h"
#include "game_object.h"
#include "game_world.h"

ChainComponent::ChainComponent(GameObject *obj) : LogicComponent(obj) {

}

ChainComponent::~ChainComponent() {

}

CompType ChainComponent::type() const {
    return kCompChain;
}

void ChainComponent::Update(float dt) {
    //테스트 움직임
    b2Vec2 move_vec(0.5f, 0);
    MoveMessage move_msg = MoveMessage::Create(move_vec);

    GameWorld *world = obj()->world();
    GameObjectPtr slave_ptr = world->FindObject(slave_id_);
    if(slave_ptr != NULL) {
        slave_ptr->OnMessage(&move_msg);
    }
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
    GameWorld *world = obj()->world();
    GameObjectPtr obj_ptr = world->FindObject(obj()->id());
    world->RequestRemoveObject(obj_ptr);
}
