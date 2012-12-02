// Ŭnicode please
#pragma once

#if SR_USE_PCH == 0
#include "cocos2d.h"
#endif

class OptionLayer : public cocos2d::CCLayer {
public:
    OptionLayer();
    virtual ~OptionLayer();
    CREATE_FUNC(OptionLayer);

    static cocos2d::CCScene *Scene();

    virtual bool init();
    void OnBack(cocos2d::CCObject *sender);
};