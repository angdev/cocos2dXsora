// Ŭnicode please
#include "stdafx.h"
#include "main_menu_layer.h"

#include "game_layer.h"
#include "option_layer.h"

using namespace std;
using namespace cocos2d;
//using namespace sora;

MainMenuLayer::MainMenuLayer() {
}

MainMenuLayer::~MainMenuLayer() {
}

bool MainMenuLayer::init() {
    if(!CCLayer::init()) {
        return false;
    }

    //타이틀 로고 
    CCSize win_size = CCDirector::sharedDirector()->getVisibleSize();
    CCSprite *logo = CCSprite::create("logo.png");
    this->addChild(logo);
    float logo_y = win_size.height-logo->getContentSize().height/2.0f - 30;
    CCPoint logo_pos = ccp(win_size.width/2.0f, logo_y);
    logo->setPosition(logo_pos);

    //기본 메뉴 구성하기
    typedef decltype(menu_selector(MainMenuLayer::OnPlayCallback)) FuncType;
    struct ButtonData {
        ButtonData(const char *label, FuncType func)
            : label(label), func(func) {}
        const char *label;
        FuncType func;
    };
    std::array<ButtonData, 6> btn_data_list = {
        ButtonData("Play", menu_selector(MainMenuLayer::OnPlayCallback)),
        ButtonData("Lab", menu_selector(MainMenuLayer::OnLabCallback)),
        ButtonData("Option", menu_selector(MainMenuLayer::OnOptionCallback)),
        ButtonData("Story", menu_selector(MainMenuLayer::OnStroyCallback)),
        ButtonData("Help", menu_selector(MainMenuLayer::OnHelpCallback)),
        ButtonData("Credit", menu_selector(MainMenuLayer::OnCreditCallback))
    };
    CCMenu *menu = CCMenu::create(NULL);
    for(auto it = btn_data_list.begin(), e = btn_data_list.end() ; it != e ; ++it) {
        const FuncType &func = it->func;
        const char *str = it->label;

        CCLabelTTF* label = CCLabelTTF::create(str, "Arial", 100);
        CCMenuItem *item = CCMenuItemLabel::create(label, this, func);
        menu->addChild(item);
    }
    menu->alignItemsVerticallyWithPadding(20);
    this->addChild(menu);
    menu->setPosition(ccp(win_size.width/2.0f, 500));

    return true;
}

cocos2d::CCScene *MainMenuLayer::scene() {
    CCScene *scene = CCScene::create();
    MainMenuLayer *layer = MainMenuLayer::create();
    scene->addChild(layer);
    scene->setTag(kSceneMainMenu);
    return scene;
}

void MainMenuLayer::OnPlayCallback(cocos2d::CCObject *sender) {
    //CCLOG("On Play");
    CCScene *game_scene = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(game_scene);
}
void MainMenuLayer::OnLabCallback(cocos2d::CCObject *sender) {
    CCLOG("On Lab");
}
void MainMenuLayer::OnOptionCallback(cocos2d::CCObject *sender) {
    //CCLOG("On Option");
    CCScene *opt_scene = OptionLayer::Scene();
    CCDirector::sharedDirector()->replaceScene(opt_scene);
}
void MainMenuLayer::OnStroyCallback(cocos2d::CCObject *sender) {
    CCLOG("On Story");
}
void MainMenuLayer::OnHelpCallback(cocos2d::CCObject *sender) {
    CCLOG("On Help");
}
void MainMenuLayer::OnCreditCallback(cocos2d::CCObject *sender) {
    CCLOG("On Credit");
}
void MainMenuLayer::OnBackCallback(cocos2d::CCObject *sender) {
    CCLOG("Exit");
}