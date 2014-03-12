#include "VideoScene.h"
#include "IOSBridge.h"
#include "LoadingScene.h"
using namespace std;

extern bool videoFinished;

CCScene* VideoScene::scene(){
    CCScene *scene = CCScene::create();
    VideoScene *layer = VideoScene::create();
    scene->addChild(layer);
    return scene;
}

bool VideoScene::init(){
    if ( !CCLayer::init() ){return false;}
    
    //IOSBridge::PlayVideoIOS();
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LoadingScene::scene()));
    //schedule(schedule_selector(VideoScene::goGameBridge));
    
    return true;
}

void VideoScene::goGameBridge()  {
    //if (videoFinished) {
        unschedule(schedule_selector(VideoScene::goGameBridge));

    //}
}

