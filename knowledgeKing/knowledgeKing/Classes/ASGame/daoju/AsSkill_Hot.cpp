#include "ASGame.h"
#include "global.h"

void ASGame::hotBuff(){
    CCSprite* ani = CCSprite::createWithSpriteFrameName("Miku_Hot_0.png");
    ani->setScale(2);
    ani->setPosition(ccp(size.width*10.5/40,size.height*75/90+winDif*2*alpha*alpha));
    addChild(ani,100,1*10000000+103);
    
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

void ASGame::hotAnimation(){
    CCSprite* ani = CCSprite::createWithSpriteFrameName("hotTriggered_0.png");
    ani->setScale(2);
    ani->setPosition(ccp(size.width*5/40,size.height*84/90+winDif*2*alpha*alpha));
    addChild(ani,100);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 15; i++) {
        string texName = "hotTriggered_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.15);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASGame::removeSprite));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,NULL);
    CCCallFunc* add = CCCallFuncN::create(this, callfuncN_selector(ASGame::hotEffected));
    CCActionInterval* effect = CCSpawn::create(seqqq,add,NULL);
    ani->runAction(effect);
}

void ASGame::hotEffected(){
    changeHeart(500+20*(Level-1),2);
}
