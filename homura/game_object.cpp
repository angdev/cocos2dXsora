// Ŭnicode please
#include "stdafx.h"
#include "game_object.h"
#include "game_component.h"
#include "drawable_component.h"
#include "logic_component.h"
#include "phy_component.h"
#include "ai_component.h"

#if SR_USE_PCH == 0
#include <array>
#endif

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
    
    //고정크기 배열, 혹은 그것과 유사한것을 배치해서 쓸데없는
    //malloc를 없애자
    std::array<GameComponent*, 4> comp_arr = {
        phy_comp_.get(),
        logic_comp_.get(),
        ai_comp_.get(),
        drawable_comp_.get(),
    };
    for(auto it = comp_arr.begin(), e = comp_arr.end() ; it != e ; ++it) {
        if(*it != NULL) {
            (*it)->Update(dt);
        }
    }
}

void GameObject::OnMessage(GameMessage *msg) {
    //쓸데없는 malloc을 줄이기 위해서 고정크기 배열 사용
    std::array<GameComponent*, 3> comp_arr = {
        drawable_comp_.get(),
        phy_comp_.get(),
        logic_comp_.get()
    };
    for(auto it = comp_arr.begin(), e = comp_arr.end() ; it != e ; ++it) {
        if(*it != NULL) {
            (*it)->OnMessage(msg);
        }
    }
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

ObjectType GameObject::Type() const {
    if(logic_comp_.get() == NULL) {
        return kCompNull;
    } else {
        return logic_comp_->type();
    }
}