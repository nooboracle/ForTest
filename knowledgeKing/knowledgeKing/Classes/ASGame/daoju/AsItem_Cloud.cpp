#include "ASGame.h"
#include "ASBot.h"
#include "IOSBridge.h"
#include "global.h"
#include "GameBaseClass.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASGame* player1;
extern ASBot* bot2;
extern ASUser* MainUser;

void ASGame::coverQuestion(int _time){
    
    if (!bot2->immune && !bot2->clouded) {
        bot2->clouded = true;
        bot2->cloudTime = _time;

        CCSprite* cloudSprite = CCSprite::create("cloud.png");
        cloudSprite->setScale(0.417);
        CCLabelTTF* Label = CCLabelTTF::create(int2string(bot2->cloudTime).c_str() , "Arial Rounded MT bold",35);
        cloudSprite->setOpacity(200);
        cloudSprite->setPosition(ccp(size.width*60/400,size.height*9/95+winDif*2*alpha*alpha));
        Label->setPosition(ccp(size.width*15/100,size.height*9/95+winDif*2*alpha*alpha));
        bot2->addChild(cloudSprite,500,2*10000000+1110);
        bot2->addChild(Label,501,2*10000000+1111);
        
        cloudMoveIn(cloudSprite,Label);
    }
}

//乌云进入
void ASGame::cloudMoveIn(CCSprite* _cloudSprite,CCLabelTTF* _Label){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("thunder.mp3", false);
    
    CCActionInterval* move = CCMoveBy::create(1,ccp(size.width*290/400,0));
    CCActionInterval* easeElastic = CCEaseElasticInOut::create(move,1);
    _cloudSprite->runAction(easeElastic);
    
    CCActionInterval* move1 = CCMoveBy::create(1,ccp(size.width*290/400,0));
    CCActionInterval* easeElastic1 = CCEaseElasticInOut::create(move1,1);
    CCCallFunc* sched1 = CCCallFuncN::create(this,callfuncN_selector(ASGame::beginCountCloudTime));
    CCSequence* sequence1 = CCSequence::create(easeElastic1,sched1,NULL);
    _Label->runAction(sequence1);
}

//乌云飞出
void ASGame::cloudMoveOut(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cloudOut.wav", false);
    
    CCSprite* cloudSprite = (CCSprite*)bot2->getChildByTag(2*10000000+1110);
    CCLabelBMFont* Label = (CCLabelBMFont*)bot2->getChildByTag(2*10000000+1111);
    
    CCActionInterval* move = CCMoveBy::create(1,ccp(size.width*1.2,0));
    CCActionInterval* easeElastic = CCEaseElasticInOut::create(move,1);
    CCCallFunc* sched1 = CCCallFuncN::create(bot2,callfuncN_selector(ASBot::removeThis));
    CCSequence* sequence1 = CCSequence::create(easeElastic,sched1,NULL);
    cloudSprite->runAction(sequence1);
    
    CCActionInterval* move1 = CCMoveBy::create(1,ccp(size.width*1.2,0));
    CCActionInterval* easeElastic1 = CCEaseElasticInOut::create(move1,1);
    CCCallFunc* sched2 = CCCallFuncN::create(bot2,callfuncN_selector(ASBot::removeThis));
    CCSequence* sequence2 = CCSequence::create(easeElastic1,sched2,NULL);
    Label->runAction(sequence2);
}


void ASGame::beginCountCloudTime(){
    if (!((GameBaseClass*)getParent())->isIntroducing)
        schedule(schedule_selector(ASGame::countCloudTime),1);
}

void ASGame::countCloudTime(){
    bot2->cloudTime -= 1;
    CCLabelTTF* label = (CCLabelTTF*)bot2->getChildByTag(2*10000000+1111);
    label->setString(int2string(bot2->cloudTime).c_str());
    
    if (bot2->cloudTime == 0) {
        unschedule(schedule_selector(ASGame::countCloudTime));
        cloudMoveOut();
        bot2->clouded = false;
    }
}