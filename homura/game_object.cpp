// Ŭnicode please
#include "stdafx.h"
#include "game_object.h"
#include "game_component.h"
#include "drawable_component.h"
#include "logic_component.h"
#include "phy_component.h"

using namespace std;

int GameObject::next_id_ = 1;

GameObject::GameObject(GameWorld *world)
    : world_(world), 
    id_(next_id_++),
    enable_(true) {
}

GameObject::~GameObject() {

}

//컴포넌트 순회할 수 있게 배열 같은 걸로 뱉는거 만들어야할듯.
void GameObject::Update(float dt) {

    //우선순위는 어떻게 되나?
    if(phy_comp_.get() != NULL)
        phy_comp_->Update(dt);

    if(logic_comp_.get() != NULL)
        logic_comp_->Update(dt);
    
    if(drawable_comp_.get() != NULL)
        drawable_comp_->Update(dt);
}

void GameObject::OnMessage(GameMessage *msg) {
 
    if(phy_comp_.get() != NULL)
        phy_comp_->OnMessage(msg);

    if(logic_comp_.get() != NULL)
        logic_comp_->OnMessage(msg);

    if(drawable_comp_.get() != NULL)
        drawable_comp_->OnMessage(msg);
}

void GameObject::set_drawable_comp(DrawableComponent *comp) {
    drawable_comp_.reset(comp);
    comp->InitMsgHandler();
}

void GameObject::set_logic_comp(LogicComponent *comp) {
    logic_comp_.reset(comp);
    comp->InitMsgHandler();
}

void GameObject::set_phy_comp(PhyComponent *comp) {
    phy_comp_.reset(comp);
    comp->InitMsgHandler();
}
