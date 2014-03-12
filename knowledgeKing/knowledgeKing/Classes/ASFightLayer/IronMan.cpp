#include "ASFightLayer.h"
#include <sys/timeb.h>
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASFightLayer::IronManPreAttack(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("ironmanattack.mp3");
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.蓄力动画
    blade = CCSprite::createWithSpriteFrameName("IronMan_hit_0.png");
    blade->setOpacity(100);
    blade->setPosition(ccp(size.width*19/50,winSize.height*9.8/20));
    addChild(blade,3);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 4; i++) {
        string texName = "IronMan_hit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    blade->runAction(pAnimate);
}

void ASFightLayer::IronManInAttack(){
    
    
    removeChild(blade);
    
    //1.镜头向右侧移动
    CCActionInterval* moveOut = CCMoveBy::create(1, ccp(-size.width*3, 0));
    CameraLayer->runAction(moveOut);
    
    //2.粒子
    skillEffect_fly = CCParticleSystemQuad::create("thunder.plist");
    skillEffect_fly->setScaleX(1.25);
    skillEffect_fly->setScaleY(0.83);
    skillEffect_fly->setPosition(ccp(size.width/3,winSize.height/2));
    addChild(skillEffect_fly,4);
    
    //3.粒子移动
    CCActionInterval* moveSlowly = CCMoveBy::create(1, ccp(size.width/20,0));
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::IronManHitEnemy));
    CCSequence* hitseq = CCSequence::create(moveSlowly,hit,NULL);
    skillEffect_fly->runAction(hitseq);
}

void ASFightLayer::IronManHitEnemy(){
    
    //1.移除技能粒子的飞行特效
    removeChild(skillEffect_fly);
    removeChild(blade);
    
    //2.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("IronMan_hit_0.png");
    hitEffect->setScale(3.5);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*35/50,winSize.height/2));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 4; i++) {
        string texName = "IronMan_hit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.2);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(2);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,NULL);
    hitEffect->runAction(seqqq);
    
    //3.英雄被击中后的动作
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq);
    
    //4.减血提示
    string damageStr = "-" + int2string(damage) + "\n" + "乌云袭击！";
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*48/70, winSize.height*5/7));
    minusHP->setRotation(20);
    addChild(minusHP,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCCallFunc* back = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::backToMainGame));
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,back,NULL);
    minusHP->runAction(seq1);
}

