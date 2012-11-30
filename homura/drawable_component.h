// Ŭnicode please
#ifndef __DRAWABLE_COMPONENT_H__
#define __DRAWABLE_COMPONENT_H__

#include "game_component.h"
#include "cocos2d.h"

//Drawable component들의 base class
class DrawableComponent : public GameComponent
{
public:
 
    DrawableComponent(GameObject *obj) : GameComponent(obj) { }
    virtual ~DrawableComponent() { }

public:
    //지금은 setter를 둔다.
    cocos2d::CCLayer *layer() { return layer_; }
    cocos2d::CCLayer *set_layer(cocos2d::CCLayer *layer);

private:
    cocos2d::CCLayer *layer_;
};


//일단 그냥 스프라이트 그리는 클래스 추가
class SimpleSpriteDrawableComponent : public DrawableComponent
{
public:
    SimpleSpriteDrawableComponent(GameObject *obj, const std::string &sprite_name);

    void InitWithRegister();

public:
    //sprite 초기화를 문자열로 해야할 듯 하지만 일단 둔다.
    cocos2d::CCSprite *sprite() { return sprite_; }
    cocos2d::CCSprite *set_sprite(cocos2d::CCSprite *sprite);


public:
    //가상 함수 구현
    void Update(float dt) {}
    void InitMsgHandler() {}

    void OnRegisterSprite() { }

private:
    std::string sprite_name_;
    cocos2d::CCSprite *sprite_;

};

#endif