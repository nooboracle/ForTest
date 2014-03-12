#include "ASBotFightLayer.h"
#include "global.h"
#include <sys/timeb.h>
using namespace std;

void ASBotFightLayer::SheldonPreAttackCritical(){
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.蓄力动画
    blade = CCSprite::createWithSpriteFrameName("Sheldon_0_0.png");
    blade->setScaleY(2.5);
    blade->setScaleX(2.5);
    blade->setPosition(ccp(size.width*0/50,winSize.height*45/70));
    addChild(blade,3);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 17; i++) {
        string texName = "Sheldon_0_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.14);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    blade->runAction(pAnimate);
}

void ASBotFightLayer::SheldonInAttackCritical(){
    
    //1.镜头向右侧移动
    CCActionInterval* moveOut = CCMoveBy::create(1.2, ccp(size.width*3, 0));
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::SheldonHitEnemyCritical));
    CCSequence* hitseq = CCSequence::create(moveOut,hit,NULL);
    CameraLayer->runAction(hitseq);
    
    //2.粒子移动
    CCActionInterval* moveSlowly = CCMoveBy::create(0.2, ccp(size.width/4,-winSize.height/6));
    CCDelayTime* delay = CCDelayTime::create(0.2);
    CCActionInterval* moveBack = CCMoveBy::create(0.25, ccp(size.width/2.7,0));
    CCSequence* seq = CCSequence::create(moveSlowly,delay,moveBack,NULL);
    blade->runAction(seq);
}

void ASBotFightLayer::SheldonHitEnemyCritical(){
    removeChild(blade);
    
    //1.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_hit_0.png");
    hitEffect->setScaleX(-6);
    hitEffect->setScaleY(6);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width/2,winSize.height/2));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 7; i++) {
        string texName = "Sheldon_hit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::removeThis));
    CCCallFunc* silent = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::SheldonSecondPeriodCritical));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,silent,NULL);
    hitEffect->runAction(seqqq);
    
    //2.英雄被击中后的动作
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq);
    
    //3.减血提示
    string damageStr = "-" + int2string(damage/2);
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*22/70, winSize.height*5/7));
    minusHP->setRotation(-20);
    addChild(minusHP,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,NULL);
    minusHP->runAction(seq1);
}

void ASBotFightLayer::SheldonSecondPeriodCritical(){
    
    //1.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_C2_0.png");
    hitEffect->setScaleX(-2);
    hitEffect->setScaleY(2);
    hitEffect->setPosition(ccp(size.width/3,winSize.height/2));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 24; i++) {
        string texName = "Sheldon_C2_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::removeThis));
    CCSequence* effect1 = CCSequence::create(pAnimate,remove,NULL);
    CCDelayTime* delay = CCDelayTime::create(1.5);
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::SheldonSecondHitEnemy));
    CCSequence* effect2= CCSequence::create(delay,hit,NULL);
    CCActionInterval* effect3 = CCSpawn::create(effect1,effect2,NULL);
    hitEffect->runAction(effect3);
}

void ASBotFightLayer::SheldonSecondHitEnemy(){
    
    //1.英雄被击中后的动作
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCCallFunc* silent = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::SilentAnimation));
    CCSequence* seq = CCSequence::create(shake1,shake2,shake3,shake4,silent,NULL);
    BotHero->runAction(seq);
    
    //2.减血提示
    string damageStr = "-" + int2string(damage/2) + "\n无法使用道具!";
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*22/70, winSize.height*5/7));
    minusHP->setRotation(-20);
    addChild(minusHP,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,NULL);
    minusHP->runAction(seq1);
}

void ASBotFightLayer::InitSD(){
    showHeroInformation();
    
    //1. 战斗大背景
    fightBG = CCSprite::createWithSpriteFrameName("bg_l.png");
    fightBG->setPosition(ccp(size.width/2, winSize.height/2));
    addChild(fightBG,-1);
    
    //2. 光晕条及动画
    halo = CCSprite::createWithSpriteFrameName("superBgCover.png");
    halo->setColor(ccc3(111,193,80));
    halo->setPosition(ccp(size.width/2, winSize.height/2));
    addChild(halo,2);
    
    CCActionInterval* scaleUp = CCScaleTo::create(0.3, 1.1);
    CCRepeatForever* repeat_halo = CCRepeatForever::create(scaleUp);
    halo->runAction(repeat_halo);
    
    //3. 粒子：飞翔的线条
    flies = CCParticleSystemQuad::create("flies.plist");
    flies->setScale(-0.417);
    flies->setPosition(ccp(-size.width/10,winSize.height/2));
    addChild(flies,1);
    
    //4.镜头层
    CameraLayer = CCLayer::create();
    CameraLayer->setPosition(CCPointZero);
    addChild(CameraLayer,2);
    
    //4. 我方超级英雄
    string HeroTexture[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    MainHero = CCSprite::createWithSpriteFrameName(HeroTexture[MainIndex-1].c_str());
    MainHero->setPosition(ccp(-size.width/4, winSize.height/2));
    CameraLayer->addChild(MainHero,3);
    
    CCActionInterval* moveIn = CCMoveBy::create(1, ccp(size.width, 0));
    CCCallFunc* skillNameAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::activeSkillNameAnimation));
    CCSequence* skillSeq = CCSequence::create(moveIn,skillNameAnimation,NULL);
    MainHero->runAction(skillSeq);
    
    //5. 敌方超级英雄
    int scale[6] = {1,1,1,-1,1,1};
    BotHero = CCSprite::createWithSpriteFrameName(HeroTexture[BotIndex-1].c_str());
    BotHero->setScaleX(scale[BotIndex-1]);
    BotHero->setPosition(ccp(-size.width*11/4, winSize.height/2));
    CameraLayer->addChild(BotHero,3);
}
