#include "ASFightLayer.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASFightLayer::LichKingPreAttack(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("lkattack.mp3", false);
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -155, -155, -155);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 155, 155, 155);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.蓄力动画
    blade = CCSprite::createWithSpriteFrameName("LichKing_0_0.png");
    blade->setScaleY(2);
    blade->setScaleX(-3);
    blade->setOpacity(150);
    blade->setColor(ccc3(140, 201, 76));
    blade->setPosition(ccp(size.width*13/50,winSize.height*9.7/20));
    addChild(blade,3);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 4; i++) {
        string texName = "LichKing_0_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.15);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCActionInterval* fadeOut = CCFadeOut::create(1.5);
    CCActionInterval* aaaa = CCSpawn::create(fadeOut,pAnimate,NULL);
    blade->runAction(aaaa);
}

void ASFightLayer::LichKingInAttack(){
    
    //1.镜头向右侧移动
    CCActionInterval* moveOut = CCMoveBy::create(2, ccp(-size.width*3, 0));
    CameraLayer->runAction(moveOut);
    
    skillEffect_fly = CCParticleSystemQuad::create("claw.plist");
    skillEffect_fly->setOpacityModifyRGB(200);
    skillEffect_fly->setScaleX(4);
    skillEffect_fly->setScaleY(2.7);
    skillEffect_fly->setPosition(ccp(size.width*2/5,winSize.height/2));
    addChild(skillEffect_fly,4);
    
    //2.粒子移动
    CCActionInterval* fadeIn = CCFadeIn::create(1.5);
    CCActionInterval* moveSlowly = CCMoveBy::create(2, ccp(size.width*0/20,0));
    CCActionInterval*bbbb = CCSpawn::create(fadeIn,moveSlowly,NULL);
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::LichKingHitEnemy));
    CCSequence* hitseq = CCSequence::create(bbbb,hit,NULL);
    skillEffect_fly->runAction(hitseq);
}

void ASFightLayer::LichKingHitEnemy(){
    
    //1.移除技能粒子的飞行特效
    removeChild(skillEffect_fly);
    removeChild(blade);
    
    //2.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("LichKing_hit_0.png");
    hitEffect->setScale(3);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*30/50,winSize.height/2));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 10; i++) {
        string texName = "LichKing_hit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
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
    string damageStr = "-" + int2string(damage) + "\n" + "答错遭受额外伤害！";
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setPosition(ccp(size.width*48/70, winSize.height*5.5/7));
    minusHP->setRotation(20);
    addChild(minusHP,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCCallFunc* back = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::backToMainGame));
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,back,NULL);
    minusHP->runAction(seq1);
    
    CCActionInterval* flash = CCTintBy::create(0.2, -255, -91, -220);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 255, 91, 220);
    CCSequence* seq321 = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq321);
    BotHero->runAction(effect);
}