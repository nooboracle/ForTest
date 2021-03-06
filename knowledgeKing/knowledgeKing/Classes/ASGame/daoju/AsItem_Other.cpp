#include "ASGame.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASGame::skipThisQuestion(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("book.wav", false);
    goToNextRound();
}

void ASGame::addShengMing(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("addShengMing.wav", false);
    
    //1.动画
    CCSprite* ani = CCSprite::createWithSpriteFrameName("addHeart_0.png");
    ani->setScale(2.5);
    ani->setPosition(ccp(size.width*12/100,size.height*92/100+winDif*2*alpha*alpha));
    addChild(ani,100);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 18; i++) {
        string texName = "addHeart_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASGame::removeSprite));
    CCSequence* effect = CCSequence::create(pAnimate,remove,NULL);
    ani->runAction(effect);
    
    //2.加血
    changeHeart(maxShengMing*0.1,2);
}

void ASGame::addMoveCount(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("addMoveCount.wav", false);
    
    //1.动画
    CCSprite* ani = CCSprite::createWithSpriteFrameName("addMove_0.png");
    ani->setScale(3);
    ani->setPosition(ccp(size.width*12/100,size.height*90/100+winDif*2*alpha*alpha));
    addChild(ani,100);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 13; i++) {
        string texName = "addMove_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASGame::removeSprite));
    CCSequence* effect = CCSequence::create(pAnimate,remove,NULL);
    ani->runAction(effect);
    
    //2.加行动力
    moveCount+=2;
    changeMoveCount();
    changeEnergyBarAnimation(200/moveToFight);
}

void ASGame::addCriticalRate(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("addCritical.wav", false);
    
    //1.数字
    criticalRateAdded = 0.25;
    criticalAddedTime = 30;
    
    //2.动画
    CCSprite* ani = CCSprite::createWithSpriteFrameName("addCritical_0.png");
    ani->setScale(2);
    ani->setPosition(ccp(size.width*13/100,size.height*94/100+winDif*2*alpha*alpha));
    addChild(ani,100);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 27; i++) {
        string texName = "addCritical_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.05);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASGame::removeSprite));
    CCSequence* effect = CCSequence::create(pAnimate,remove,NULL);
    ani->runAction(effect);
    
    //3.开始计时
    schedule(schedule_selector(ASGame::CriticalRateCounter),1);
}

void ASGame::CriticalRateCounter(){
    if (criticalAddedTime == 0)
        unschedule(schedule_selector(ASGame::CriticalRateCounter));
    criticalAddedTime--;
}