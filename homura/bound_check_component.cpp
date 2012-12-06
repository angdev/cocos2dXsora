// Ŭnicode please
#include "stdafx.h"
#include "game_object.h"
#include "game_world.h"
#include "bound_check_component.h"

CompType BoundCheckComponent::type() const {
    return kCompBoundCheck;
}

void BoundCheckComponent::Update(float dt) {
    BoundCheckMessage msg = BoundCheckMessage::Create(window_size_);
    //이 객체가 포함된 월드에 메시지 브로드캐스팅
    obj()->world()->OnMessage(&msg);
}