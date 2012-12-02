#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class GLESDebugDraw;
class b2World;

class HelloWorld : public cocos2d::CCLayer {
public:
    HelloWorld();
    virtual ~HelloWorld();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

    //터치 함수 적절히 만들기
public:
    virtual void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
    virtual void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

public:
    void draw();
    void update(float dt);

    void AddNewBodyAtPosition(const cocos2d::CCPoint &p);

    //box2d 대응
private:
    b2World* world_;
    GLESDebugDraw* debug_draw_;

    bool InitPhy();
};

#endif // __HELLOWORLD_SCENE_H__
