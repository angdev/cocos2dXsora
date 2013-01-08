// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class GameWorld;
class GameStage;
class GameObject;
class GameObjectFactory;
class GameInfoLayer;

#define PLAYER_START_POINT_X 350
#define PLAYER_START_POINT_Y 100

//게임 시작 누르면 뭐 스테이지 선택 이런 레이어로 넘어갔다가
//선택하면 여기로 넘어옴
//이 레이어는 게임 플레이 부분만 담당하니
enum GameState {
    kGameReadyState,
    kGameProgressState,
    kGameVictoryState,
    kGameOverState,
};

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
    void ReadyPlayer(GameObject *player);

public:
    virtual void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

private:
    GameObject *CreatePlayer();
    void MoveBodyByDelta(const float &dx, const float &dy);

private:
    std::unique_ptr<GameWorld> world_;
    //TODO
    //플레이어 정보를 따로 담습니다.
    GameObject *player_;
    
    GameObjectFactory *factory_;
    GameStage *stage_;
    GameState state_;

    //임시로 버튼을 여기에 저장
    //Touchable Sprite를 만들어봐야겠다
    cocos2d::CCSprite *lethal_btn;
    cocos2d::CCSprite *lethal_btn2;
};