#include "ASFightLayer.h"
#include "global.h"
using namespace std;

void ASFightLayer::LuffyPreAttack(){
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.蓄力动画
    blade = CCSprite::createWithSpriteFrameName("Luffy_0_0.png");
    blade->setScale(8);
    blade->setScale(-8);
    blade->setPosition(ccp(-size.width*15/8,size.height*18.5/8));
    addChild(blade,3);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 8; i++) {
        string texName = "Luffy_0_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    blade->runAction(pAnimate);
}

void ASFightLayer::LuffyInAttack(){
    
    removeChild(blade);

    
    CCActionInterval* moveOut = CCMoveBy::create(1, ccp(-size.width*3, 0));
    CCCallFunc* attack = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::LuffyAttackAnimation));
    CCSequence* seq = CCSequence::create(moveOut,attack,NULL);
    CameraLayer->runAction(seq);
}

void ASFightLayer::LuffyAttackAnimation(){
    
    //1.攻击动画
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Luff_A_0.png");
    hitEffect->setScale(3);
    //mhitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*8/8,size.height*10/50));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 11; i++) {
        string texName = "Luff_A_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.2);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::LuffyHitEnemy));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,hit,NULL);
    hitEffect->runAction(seqqq);
    
    //2.英雄被击中后的动作
    CCDelayTime* delay = CCDelayTime::create(0.5);
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq = CCSequence::create(delay,shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq);
    
    //3.减血提示
    minusHP = CCLabelTTF::create("-1000HP\n-1SP", "Arial", 62.5);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*48/70, size.height*5/7));
    minusHP->setRotation(20);
    addChild(minusHP,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,NULL);
    minusHP->runAction(seq1);
}

void ASFightLayer::LuffyHitEnemy(){
    
    //1.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Luffy_hit_0.png");
    hitEffect->setScale(2);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*6/8,size.height*10/50));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 27; i++) {
        string texName = "Luffy_hit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.2);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCCallFunc* back = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::backToMainGame));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,back,NULL);
    hitEffect->runAction(seqqq);
}