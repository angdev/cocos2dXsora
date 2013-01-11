// Ŭnicode please
#include "stdafx.h"

#include "app_delegate.h"
#include "CCEGLView.h"

using namespace cocos2d;

int main(int argc, char *argv[])
/*
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
                       */
{
    //UNREFERENCED_PARAMETER(hPrevInstance);
    //UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    //eglView->setFrameSize(2048, 1536);
    //eglView->setFrameSize(1024, 768);
    //eglView->setFrameSize(480, 320);
    eglView->setFrameSize(720, 1280);

    // The resolution of ipad3 is very large. In general, PC's resolution is smaller than it.
    // So we need to invoke 'setFrameZoomFactor'(only valid on desktop(win32, mac, linux)) to make the window smaller.
    eglView->setFrameZoomFactor(0.4f);
    //eglView->setFrameZoomFactor(2.0f);
    //eglView->setFrameZoomFactor(1.0f);
    return CCApplication::sharedApplication()->run();
}