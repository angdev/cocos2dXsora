// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class GameWorld;
class GameStage;
class GameObject;
class GameObjectFactory;

class GameLayer : public cocos2d::CCLayer {
public:
    GameLayer();
    virtual ~GameLayer();
    CREATE_FUNC(GameLayer);

    virtual bool init();
    static cocos2d::CCScene *scene();

public:
    void update(float dt);
    void EndStage();

public:
    GameObject *player();
    void set_player(GameObject *player);

public:
    virtual void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

private:
    GameObject *CreatePlayer();

private:
    std::unique_ptr<GameWorld> world_;
    //TODO
    //플레이어 정보를 따로 담습니다.
    GameObject *player_;
    
    void MoveBodyByDelta(const float &dx, const float &dy);

    //이거 대신 스테이지가 들어감
    cocos2d::CCLayer *simple_layer_;

    GameObjectFactory *factory_;
    GameStage *stage_;
};