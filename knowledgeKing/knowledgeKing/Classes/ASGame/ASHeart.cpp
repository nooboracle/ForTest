#include "ASGame.h"
#include "ASBot.h"
#include "global.h"
#include "GameBaseClass.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASBot* bot2;
extern ASUser* MainUser;

void ASGame::HeartBar(){
    
    //1.血条
    CCSprite* heartBarSprite = CCSprite::createWithSpriteFrameName("newLifeBar.png");
    CCProgressTimer* heartBar = CCProgressTimer::create(heartBarSprite);
    heartBar->setPosition(ccp(size.width*23.08/80,size.height*85.23/90+winDif*2*alpha*alpha));
    heartBar->setType(kCCProgressTimerTypeBar);
    heartBar->setMidpoint(ccp(0,0.5));
    heartBar->setBarChangeRate(ccp(1, 0));
    heartBar->setPercentage(100);
    addChild(heartBar,4,1*10000000+2222);
    
    //2.生命数字
    string HeartStr = int2string(shengming)  + "/" + int2string(maxShengMing);
    CCLabelTTF* userHeartNumber= CCLabelTTF::create(HeartStr.c_str(), "Arial Rounded MT bold",25);
    userHeartNumber->setPosition(ccp(size.width*26/80,size.height*82/90+winDif*2*alpha*alpha));
    addChild(userHeartNumber,4,1*10000000+2223);
}

void ASGame::changeHeart(int _shengming,int _source){
    
    shengming += _shengming;
    if (shengming<=0)
        shengming = 0;
    
    //1.血条
    CCProgressTimer* heartBar = (CCProgressTimer*)this->getChildByTag(1*10000000+2222);
    CCProgressFromTo* action = CCProgressFromTo::create(0.5, heartBar->getPercentage(), (float)shengming/(float)maxShengMing*100);
    heartBar->runAction(action);
    if ((float)shengming <= 0.4*(float)maxShengMing){
        heartBar->getSprite()->setColor(ccc3(237, 45, 37));
        
        if(!MainUser->muted){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(heartSoundEffect);
            heartSoundEffect = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("noHeart.wav", true);
        }
    }else{
        heartBar->getSprite()->setColor(ccc3(255, 255, 255));
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(heartSoundEffect);
    }
    
    //2.生命数字
    string HeartStr = int2string(shengming)  + "/" + int2string(maxShengMing);
    CCLabelTTF* heartLabel = (CCLabelTTF*)this->getChildByTag(playerNumber*10000000+2223);
    heartLabel->setString(HeartStr.c_str());

    //3.判断胜利条件
    if (shengming == 0 && !((GameBaseClass*)getParent())->gameover){
        ((GameBaseClass*)getParent())->gameover = true;
        //1.禁用各种按钮
        CCMenu* skillMenu = (CCMenu*)this->getChildByTag(1*10000000+27);
        CCMenu* optionMenu = (CCMenu*)this->getChildByTag(1*10000000+12);
        CCMenu* itemMenu = (CCMenu*)this->getChildByTag(1*10000000+77776);
        skillMenu->setTouchEnabled(false);
        optionMenu->setTouchEnabled(false);
        itemMenu->setTouchEnabled(false);
        
        //2.停止所有监听
        unscheduleAllSelectors();
        bot2->unscheduleAllSelectors();
        
        //3.结束页面
        ((GameBaseClass*)getParent())->GameResult(1);
    }
}
