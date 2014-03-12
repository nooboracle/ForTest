#include "AppDelegate.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "VideoScene.h"
#include "testEffect.h"
#include "LoadingScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate(){}
AppDelegate::~AppDelegate(){}

bool AppDelegate::applicationDidFinishLaunching(){
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    pDirector->setDisplayStats(false);
    pDirector->setAnimationInterval(1.0 / 60);
    CCScene *pScene = LoadingScene::scene();
    pDirector->runWithScene(pScene);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
   
    if (size.height == 1136)  //iphone 5-5s
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(640, 1136, kResolutionFixedHeight);
    else if(size.height == 960) //iphone 4－4s
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(640, 960, kResolutionFixedHeight);
    else if(size.height == 1024)  //ipad 1-2－mini
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(640,854, kResolutionFixedHeight);
    else if(size.height == 2048) //ipad 3-4-5-mini2
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(640, 854, kResolutionFixedHeight);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground(){
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground(){
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
