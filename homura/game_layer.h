﻿// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class GameWorld;

class GameLayer : public cocos2d::CCLayer {
public:
    GameLayer();
    virtual ~GameLayer();
    CREATE_FUNC(GameLayer);

    virtual bool init();
    static cocos2d::CCScene *scene();

public:
    virtual void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

public:
    void update(float dt);

private:
    std::unique_ptr<GameWorld> world_;
    GameObject *player_;

    void AddNewBodyAtPosition(const cocos2d::CCPoint &p);
    void MoveBodyByDelta(const float &dx, const float &dy);

    cocos2d::CCLayer *simple_layer_;
};