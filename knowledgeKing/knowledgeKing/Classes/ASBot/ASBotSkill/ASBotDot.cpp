#include "ASBot.h"
#include "global.h"
#include "ASGame.h"
#include "GameBaseClass.h"

extern ASGame* player1;

void ASBot::dotAnimation(){
    
    //1.头像上的动画
    CCSprite* dotAnimation = CCSprite::createWithSpriteFrameName("LichKing_DOT_0.png");
    dotAnimation->setPosition(ccp(size.width*1.15/9.7,size.height*10/11+winDif*2*alpha*alpha));
    player1->addChild(dotAnimation,100,1*10000000+102);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 28; i++) {
        string texName = "LichKing_DOT_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    dotAnimation->runAction(pAnimate);
    
    //2.背景绿光
    CCSprite* bg = (CCSprite*)((GameBaseClass*)getParent())->getChildByTag(999);
    CCActionInterval* flash = CCTintBy::create(0.5, -175, -45, -175);
    CCDelayTime* delay = CCDelayTime::create(1);
    CCActionInterval* flash1 = CCTintBy::create(0.5, 175, 45, 175);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    bg->runAction(effect);
}

void ASBot::stopDotAnimation(){
    CCSprite* bg = (CCSprite*)((GameBaseClass*)getParent())->getChildByTag(999);
    bg->stopAllActions();
    CCActionInterval* flash = CCTintTo::create(0.5, 255, 255, 255);
    bg->runAction(flash);
}