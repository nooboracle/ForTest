#include "ASBot.h"
#include "global.h"

void ASBot::botHotBuff(){
    
    CCSprite* ani = CCSprite::createWithSpriteFrameName("Miku_Hot_0.png");
    ani->setScale(2);
    ani->setPosition(ccp(size.width*41.5/40,size.height*5.5/100+winDif*2*alpha*alpha));
    addChild(ani,100,2*10000000+103);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 19; i++) {
        string texName = "Miku_Hot_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    ani->runAction(pAnimate);
}

void ASBot::botHotAnimation(){
    CCSprite* ani = CCSprite::createWithSpriteFrameName("hotTriggered_0.png");
    ani->setScale(2);
    ani->setPosition(ccp(size.width*35.2/40,size.height*11.2/100+winDif*2*alpha*alpha));
    addChild(ani,100);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 15; i++) {
        string texName = "hotTriggered_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASBot::removeThis));
    CCSequence* seq = CCSequence::create(pAnimate,remove,NULL);
    CCCallFunc* add = CCCallFuncN::create(this, callfuncN_selector(ASBot::botHotEffected));
    CCActionInterval* effect = CCSpawn::create(seq,add,NULL);
    ani->runAction(effect);
}

void ASBot::botHotEffected(){   changeHeart(500+20*(Level-1),2);    }