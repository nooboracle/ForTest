#include "ASGame.h"
#include "global.h"
#include "IOSBridge.h"
using namespace std;

void ASGame::reborn(CCObject* sender){
    removeChild((CCNode*)sender,true);
    changeHeart(5);
    goToNextRound();
}

void ASGame::rebornAnimation(){
    
    unschedule(schedule_selector(ASGame::timeIsUp));
    removeChildByTag(playerNumber*10000000+2);
    removeChildByTag(playerNumber*10000000+4);
    removeChildByTag(playerNumber*10000000+5);
    removeChildByTag(playerNumber*10000000+6);
    removeChildByTag(playerNumber*10000000+5);
    
    CCLabelTTF* label = CCLabelTTF::create("复活动画","Arial",40);
    label->setPosition(ccp(320,240));
    addChild(label,4);
    
    CCDelayTime* delay = CCDelayTime::create(1);
    CCCallFunc* rebornAction = CCCallFuncN::create(this,callfuncN_selector(ASGame::reborn));
    CCSequence* seq = CCSequence::create(delay,rebornAction,NULL);
    label->runAction(seq);
}
