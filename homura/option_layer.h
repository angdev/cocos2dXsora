// Ŭnicode please
#pragma once

class OptionLayer : public cocos2d::CCLayer {
public:
    OptionLayer();
    virtual ~OptionLayer();
    CREATE_FUNC(OptionLayer);

    static cocos2d::CCScene *Scene();

    virtual bool init();
    void OnBack(cocos2d::CCObject *sender);
};