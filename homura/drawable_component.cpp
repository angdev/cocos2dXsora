// Ŭnicode please
#include "stdafx.h"
#include "drawable_component.h"

USING_NS_CC;

SimpleSpriteDrawableComponent::SimpleSpriteDrawableComponent(GameObject *obj, const std::string &sprite_name) 
    : DrawableComponent(obj), sprite_name_(sprite_name) {
        
}

void SimpleSpriteDrawableComponent::InitWithRegister()
{
    if(sprite_name_.empty())
        return;

    sprite_ = CCSprite::create(sprite_name_.c_str());
    layer()->addChild(sprite_);
}