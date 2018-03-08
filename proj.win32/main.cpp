#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
USING_NS_CC;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	eglView->setViewName("HNU");
    eglView->setFrameSize(480, 800);
    return  ( CCApplication::sharedApplication()->run());
}

