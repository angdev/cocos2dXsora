// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class MainMenuLayer : public cocos2d::CCLayer {
public:
    MainMenuLayer();
    virtual ~MainMenuLayer();
    CREATE_FUNC(MainMenuLayer);

    virtual bool init();
    static cocos2d::CCScene *scene();
    
    //menu callback
private:
    virtual void OnPlayCallback(cocos2d::CCObject *sender);
    virtual void OnLabCallback(cocos2d::CCObject *sender);
    virtual void OnOptionCallback(cocos2d::CCObject *sender);
    virtual void OnStroyCallback(cocos2d::CCObject *sender);
    virtual void OnHelpCallback(cocos2d::CCObject *sender);
    virtual void OnCreditCallback(cocos2d::CCObject *sender);

    virtual void keyBackClicked();
};
