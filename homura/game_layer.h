// Ŭnicode please
#pragma once



class GameLayer : public cocos2d::CCLayer {
public:
    GameLayer();
    virtual ~GameLayer();
    CREATE_FUNC(GameLayer);

    virtual bool init();
    static cocos2d::CCScene *scene();

public:
    virtual void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

public:
    void draw();
    void update(float dt);

private:
    std::unique_ptr<GameWorld> world_;
    void AddNewBodyAtPosition(const cocos2d::CCPoint &p);
};