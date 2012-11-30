// Ŭnicode please
#include "stdafx.h"
#include "game_object_factory.h"

#include "drawable_component.h"
#include "game_world.h"

USING_NS_CC;

GameObject* GameObjectFactory::Create(const TestSpriteObjectHeader &header) {
    GameObject *obj = new GameObject(world_);

    SimpleSpriteDrawableComponent *sprite_comp = new SimpleSpriteDrawableComponent(obj, "HelloWorld.png");
    CCSprite* sprite = sprite_comp->sprite();

    obj->set_drawable_comp(sprite_comp);

    return obj;
}