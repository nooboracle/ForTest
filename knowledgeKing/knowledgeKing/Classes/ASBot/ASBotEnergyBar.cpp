#include "ASBot.h"
#include "global.h"

void ASBot::energyBarAnimation(){
    
    //1.显示行动力的圆形底座
    CCSprite* energyBall = CCSprite::createWithSpriteFrameName("newEnergyContainer.png");
    energyBall->setScaleX(-1);
    energyBall->setPosition(ccp(size.width*62.62/80,size.height*3.85/90+winDif*2*alpha*alpha));
    addChild(energyBall,4);
    
    //2.行动力数量
    string perStr = int2string(moveCount);
    CCLabelTTF* per = CCLabelTTF::create(perStr.c_str(),"Arial Rounded MT Bold",32);
    per->setPosition(ccp(size.width*62.3/80,size.height*3.87/90+winDif*2*alpha*alpha));
    addChild(per,5,2*10000000+26);
    
    //3.能量条
    CCSprite* energyBarSprite = CCSprite::createWithSpriteFrameName("newEnergyBar.png");
    CCProgressTimer* energyBar = CCProgressTimer::create(energyBarSprite);
    energyBar->setScaleX(-1);
    energyBar->setPosition(ccp(size.width*71.6/80,size.height*5.89/90+winDif*2*alpha*alpha));
    energyBar->setType(kCCProgressTimerTypeRadial);
    energyBar->setPercentage(moveCount*20);
    addChild(energyBar,4,2*10000000+20);
}

void ASBot::changeEnergyBarAnimation(int _dif){
    
    CCProgressTimer* energyBar = (CCProgressTimer*)this->getChildByTag(2*10000000+20);
    int percentage = energyBar->getPercentage();
    
    CCProgressFromTo* action = CCProgressFromTo::create(0.015*(abs(moveCount*100/moveToFight-percentage)), percentage, moveCount*100/moveToFight);
    CCSequence* seq = CCSequence::create(action,NULL);
    energyBar->runAction(seq);
}