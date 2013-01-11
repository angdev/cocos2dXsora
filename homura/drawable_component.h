// Ŭnicode please
#ifndef __DRAWABLE_COMPONENT_H__
#define __DRAWABLE_COMPONENT_H__

#include "game_component.h"
#include "cocos2d.h"

//Drawable component들의 base class
class DrawableComponent : public GameComponent {
public:
    DrawableComponent(GameObject *obj, cocos2d::CCNode *parent);
    virtual ~DrawableComponent() { }

    virtual CompType type() const { return kCompDrawable; }

public:
    //지금은 setter를 둔다.
    cocos2d::CCNode *parent() { return parent_; }
private:
    cocos2d::CCNode *parent_;
};


//일단 그냥 CCNode 그리는 클래스 추가
//sprite가 아니라 node로 한 이유는 범용성을 위해서
class NodeDrawableComponent : public DrawableComponent {
public:
    NodeDrawableComponent(GameObject *obj, cocos2d::CCNode *parent, cocos2d::CCNode *node_);
    virtual ~NodeDrawableComponent();

public:
    //가상 함수 구현
    virtual void Update(float dt);
    virtual void InitMsgHandler();

    cocos2d::CCNode *node() { return node_; }

private:
    cocos2d::CCNode *node_;
};

#endif