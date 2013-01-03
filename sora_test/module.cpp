// Ŭnicode please
#include "stdafx.h"

using namespace cocos2d;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class TestAppDelegate : private cocos2d::CCApplication {
public:
    TestAppDelegate() {
    }

    ~TestAppDelegate()  {
    }

    bool applicationDidFinishLaunching() {    
        // initialize director
        //CCDirector *pDirector = CCDirector::sharedDirector();

        //pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
        //pDirector->setProjection(kCCDirectorProjection2D);
        //CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();

        //if (screenSize.height > 768) {
        //    CCFileUtils::sharedFileUtils()->setResourceDirectory("ipadhd");
        //} else if (screenSize.height > 320) {
        //    CCFileUtils::sharedFileUtils()->setResourceDirectory("ipad");
        //} else {
        //    CCFileUtils::sharedFileUtils()->setResourceDirectory("iphone");  
        //}
        CCFileUtils::sharedFileUtils()->setResourceDirectory("iphone");  
        return true;
    }

    // This function will be called when the app is inactive. When comes a phone call,it's be invoked too
    void applicationDidEnterBackground() {
        CCDirector::sharedDirector()->stopAnimation();

        // if you use SimpleAudioEngine, it must be pause
        // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }

    // this function will be called when the app is active again
    void applicationWillEnterForeground() {
        CCDirector::sharedDirector()->startAnimation();

        // if you use SimpleAudioEngine, it must resume here
        // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
};

TestAppDelegate *app = NULL;
   

TEST_MODULE_INITIALIZE(ModuleInitialize) {
    Logger::WriteMessage("In Module Initialize");
    app = new TestAppDelegate();
    //app.applicationDidFinishLaunching();
}

TEST_MODULE_CLEANUP(ModuleCleanup) {
    Logger::WriteMessage("In Module Cleanup");
    delete(app);
}
