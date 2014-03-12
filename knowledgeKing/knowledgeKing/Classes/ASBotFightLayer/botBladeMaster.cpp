#include "ASBotFightLayer.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASBotFightLayer::BladeMasterPreAttack(){
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bmattack.mp3", false);
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.蓄力动画
    blade = CCSprite::createWithSpriteFrameName("Blade_0_0.png");
    blade->setRotation(-90);
    blade->setScaleY(2);
    blade->setScaleX(-1.2);
    blade->setOpacity(100);
    blade->setPosition(ccp(size.width*21/50,winSize.height/2));
    addChild(blade,3);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 6; i++) {
        string texName = "Blade_0_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    blade->runAction(pAnimate);
}

void ASBotFightLayer::BladeMasterInAttack(){
    
    //1.镜头向右侧移动
    CCActionInterval* moveOut = CCMoveBy::create(1.3, ccp(size.width*3, 0));
    CameraLayer->runAction(moveOut);
    
    skillEffect_fly = CCParticleSystemQuad::create("knife4.plist");
    skillEffect_fly->setScaleX(-2.5);
    skillEffect_fly->setScaleY(1.7);
    skillEffect_fly->setPosition(ccp(size.width*3/5,winSize.height/2));
    addChild(skillEffect_fly,4);

    
    //2.粒子移动
    CCActionInterval* moveSlowly = CCMoveBy::create(1.3, ccp(-size.width/20,0));
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::BladeMasterHitEnemy));
    CCSequence* hitseq = CCSequence::create(moveSlowly,hit,NULL);
    skillEffect_fly->runAction(hitseq);
}

void ASBotFightLayer::BladeMasterHitEnemy(){
    
    //1.移除技能粒子的飞行特效
    removeChild(skillEffect_fly);
    removeChild(blade);
    
    //2.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Blade_hit_0.png");
    hitEffect->setScaleY(5);
    hitEffect->setScaleX(-5);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width/2,winSize.height/2));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 2; i++) {
        string texName = "Blade_hit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.2);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::removeThis));
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
    string damageStr = "-" + int2string(damage);
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*22/70, winSize.height*5/7));
    minusHP->setRotation(-20);
    addChild(minusHP,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCCallFunc* back = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::backToMainGame));
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,back,NULL);
    minusHP->runAction(seq1);
}