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
extern ASUser* MainUser;

void ASBot::coverQuestion(int _time){
    
    if (!player1->immune && !player1->clouded) {

        player1->clouded = true;
        player1->cloudTime = _time;
        
        CCSprite* cloudSprite = CCSprite::create("cloud.png");
        CCLabelTTF* Label = CCLabelTTF::create(int2string(player1->cloudTime).c_str() , "Arial Rounded MT bold",35);
        
        cloudSprite->setScale(1.3);
        Label->setScale(1.3);
        cloudSprite->setPosition(ccp(-size.width/2,size.height*4.8/8+winDif*2));
        Label->setPosition(ccp(-size.width/2,size.height*4.7/8+winDif*2));
        Label->setColor(ccc3(31, 31, 31));
        player1->addChild(cloudSprite,500,1*10000000+1110);
        player1->addChild(Label,501,1*10000000+1111);
        
        cloudMoveIn(cloudSprite,Label);
    }
}

//乌云进入
void ASBot::cloudMoveIn(CCSprite* _cloudSprite,CCLabelTTF* _Label){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("thunder.mp3", false);
    
    CCActionInterval* move = CCMoveBy::create(1,ccp(size.width,0));
    CCActionInterval* easeElastic = CCEaseElasticInOut::create(move,1);
    _cloudSprite->runAction(easeElastic);
    
    CCActionInterval* move1 = CCMoveBy::create(1,ccp(size.width,0));
    CCActionInterval* easeElastic1 = CCEaseElasticInOut::create(move1,1);
    CCCallFunc* sched1 = CCCallFuncN::create(this,callfuncN_selector(ASBot::beginCountCloudTime));
    CCSequence* sequence1 = CCSequence::create(easeElastic1,sched1,NULL);
    _Label->runAction(sequence1);
}

//乌云飞出
void ASBot::cloudMoveOut(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("cloudOut.wav", false);
    
    CCSprite* cloudSprite = (CCSprite*)player1->getChildByTag(1*10000000+1110);
    CCLabelBMFont* Label = (CCLabelBMFont*)player1->getChildByTag(1*10000000+1111);
    
    CCActionInterval* move = CCMoveBy::create(1,ccp(size.width,0));
    CCActionInterval* easeElastic = CCEaseElasticInOut::create(move,1);
    CCCallFunc* sched = CCCallFuncN::create(player1,callfuncN_selector(ASGame::removeSprite));
    CCSequence* sequence = CCSequence::create(easeElastic,sched,NULL);
    cloudSprite->runAction(sequence);
    
    CCActionInterval* move1 = CCMoveBy::create(1,ccp(size.width,0));
    CCActionInterval* easeElastic1 = CCEaseElasticInOut::create(move1,1);
    CCCallFunc* sched1 = CCCallFuncN::create(player1,callfuncN_selector(ASGame::removeSprite));
    CCSequence* sequence1 = CCSequence::create(easeElastic1,sched1,NULL);
    Label->runAction(sequence1);
}


void ASBot::beginCountCloudTime(){
    if (!((GameBaseClass*)getParent())->isIntroducing)
        schedule(schedule_selector(ASBot::countCloudTime),1);
}

void ASBot::countCloudTime(){
    
    cout<<player1->cloudTime<<endl;
    player1->cloudTime -= 1;
    CCLabelTTF* label = (CCLabelTTF*)player1->getChildByTag(1*10000000+1111);
    label->setString(int2string(player1->cloudTime).c_str());
    
    if (player1->cloudTime<=0) {
        unschedule(schedule_selector(ASBot::countCloudTime));
        cloudMoveOut();
        player1->clouded = false;
    }
}

void ASBot::removeCloud(CCNode* sender){   removeChild(sender);    }