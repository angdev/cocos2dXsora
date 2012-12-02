// Ŭnicode please
#include "stdafx.h"
#include "app_delegate.h"
#include "homura/app_macros.h"

#include "homura/game_console.h"
#include "sora/data_init.h"

#include "sora/data_define.h"
#include "sora/data_helper.h"

#include "homura/hello_world_scene.h"
#include "homura/main_menu_layer.h"
#include "homura/game_layer.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // RTTI 초기화. 가장 빨리 하는게 정신 건강에 좋다
    init_generated_data();

    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();

    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    pDirector->setProjection(kCCDirectorProjection2D);
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();

    if (screenSize.height > 768)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory("ipadhd");
        pDirector->setContentScaleFactor(1536.0f/kDesignResolutionSize_height);
    }
    else if (screenSize.height > 320)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory("ipad");
        pDirector->setContentScaleFactor(768.0f/kDesignResolutionSize_height);
    }
    else
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory("iphone");
        pDirector->setContentScaleFactor(320.0f/kDesignResolutionSize_height);
    }

    //CCEGLView::sharedOpenGLView()->setDesignResolutionSize(kDesignResolutionSize_width, kDesignResolutionSize_height, kResolutionNoBorder);

    //안드로이드로 맞출 기본 해상도
    //작업은 720*1280을 기본으로 한다
    CCFileUtils::sharedFileUtils()->setResourceDirectory("720");
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(720, 1280, kResolutionNoBorder);
    pDirector->setContentScaleFactor(1.0f);


    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

#if CC_CONSOLE
    //게임콘솔 설정. director보다 나중에 초기화되야하니까 여기에서 처리하자
    //전역변수로 초기화까지 한방에 해버리면 순서 통제가 안되서 여기에 따로둠
    GameConsole::Init();
#endif

    // create a scene. it's an autorelease object
    //CCScene *scene = HelloWorld::scene();

    CCScene *scene = MainMenuLayer::scene();

    //게임을 바로 시작할떄 쓰기
    //CCScene *scene = GameLayer::scene();

    // run
    pDirector->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

