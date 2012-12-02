// Ŭnicode please
#include "stdafx.h"
#include "option_layer.h"
#include "main_menu_layer.h"

using namespace cocos2d;
using namespace std;

OptionLayer::OptionLayer() {
}
OptionLayer::~OptionLayer() {
}

cocos2d::CCScene *OptionLayer::Scene() {
    CCScene *scene = CCScene::create();
    scene->setTag(kSceneOption);
    OptionLayer *layer = OptionLayer::create();
    scene->addChild(layer);
    return scene;
}

bool OptionLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }
    
    this->setKeypadEnabled(true);

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
        "CloseNormal.png",
        "CloseSel_ected.png",
        this,
        menu_selector(OptionLayer::OnBack));

    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
        origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    return true;
}

void OptionLayer::OnBack(cocos2d::CCObject *sender) {
    CCScene *menu_scene = MainMenuLayer::scene();
    CCDirector::sharedDirector()->replaceScene(menu_scene);
}