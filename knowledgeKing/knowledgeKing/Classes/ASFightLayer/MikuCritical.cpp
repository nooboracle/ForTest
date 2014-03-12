#include "ASFightLayer.h"
#include <sys/timeb.h>
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;
extern ASUser* MainUser;

void ASFightLayer::MikuPreAttackCritical(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("mikuCritical.mp3");
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.起手
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("miku_C1_0.png");
    hitEffect->setScale(3);
    hitEffect->setPosition(ccp(size.width/4,winSize.height/2+120));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 17; i++) {
        string texName = "miku_C1_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCSequence* effect1 = CCSequence::create(pAnimate,remove,NULL);
    hitEffect->runAction(effect1);
}

void ASFightLayer::MikuInAttackCritical(){
    
    //1.镜头向右侧移动
    CCActionInterval* moveOut = CCMoveBy::create(0.5, ccp(-size.width*13/4, 0));
    CCCallFunc* attack = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::MikuBigAttack));
    CCSequence* seq = CCSequence::create(moveOut,attack,NULL);
    CameraLayer->runAction(seq);
}

void ASFightLayer::MikuBigAttack(){
    CCPoint pos[6] = {
        ccp(size.width/4,winSize.height*2/3),
        ccp(size.width/2,winSize.height*4/5),
        ccp(size.width*3/4,winSize.height*2/3),
        ccp(size.width*3/4, winSize.height/3),
        ccp(size.width/2, winSize.height/5),
        ccp(size.width/4, winSize.height/3)
    };
    
    int rotation[6] = {-225,-180,-135,-45,0,45};
    
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("miku_C_0.png");
    hitEffect->setRotation(rotation[mikuCriticalCounter]);
    hitEffect->setScale(1.5);
    hitEffect->setPosition(pos[mikuCriticalCounter]);
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 34; i++) {
        string texName = "miku_C_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    if (mikuCriticalCounter < 6){
        mikuCriticalCounter++;
        
        CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
        CCSequence* effect1 = CCSequence::create(pAnimate,remove,NULL);
        CCDelayTime* delay1 = CCDelayTime::create(1.05);
        CCCallFunc* attack = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::MikuBigAttack));
        CCSequence* effect2 = CCSequence::create(delay1,attack,NULL);
        CCDelayTime* delay2 = CCDelayTime::create(2.0);
        CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::MikuHitEnemyCritical));
        CCSequence* effect3 = CCSequence::create(delay2,hit,NULL);
        CCActionInterval* effect4 = CCSpawn::create(effect1,effect2,effect3,NULL);
        hitEffect->runAction(effect4);
    }else{
        mikuCriticalCounter = 0;
        CCDelayTime* delay = CCDelayTime::create(3.5);
        CCCallFuncN* goOn = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::backToMainGame));
        runAction(CCSequence::create(delay,goOn,NULL));
    }
}

void ASFightLayer::MikuHitEnemyCritical(){
    //1.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_hit_0.png");
    hitEffect->setScaleY(5);
    hitEffect->setScaleX(-5);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*12/20,winSize.height/2));
    addChild(hitEffect,3);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 7; i++) {
        string texName = "Sheldon_hit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.2);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,NULL);
    hitEffect->runAction(seqqq);
    
    //2.英雄被击中后的动作
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq);
    
    //3.减血提示
    string damageStr = "-" + int2string(damage/6);
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*48/70, winSize.height*5/7));
    minusHP->setRotation(20);
    addChild(minusHP,10);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,NULL);
    minusHP->runAction(seq1);
}

void ASFightLayer::InitMK(){
    
    //1. 战斗大背景
    fightBG = CCSprite::createWithSpriteFrameName("bg_l.png");
    fightBG->setPosition(ccp(size.width/2, winSize.height/2));
    addChild(fightBG,-1);
    
    //2.镜头层
    CameraLayer = CCLayer::create();
    CameraLayer->setPosition(CCPointZero);
    addChild(CameraLayer,3);
    
    //3. 光晕条及动画
    halo = CCSprite::createWithSpriteFrameName("superBgCover.png");
    halo->setPosition(ccp(size.width/2, winSize.height/2));
    addChild(halo,2);
    
    CCActionInterval* scaleUp = CCScaleTo::create(0.3, 1.1);
    CCRepeatForever* repeat_halo = CCRepeatForever::create(scaleUp);
    halo->runAction(repeat_halo);
    
    //3. 粒子：飞翔的线条
    flies = CCParticleSystemQuad::create("fliesUp.plist");
    flies->setPosition(ccp(size.width/2,winSize.height*11/10));
    addChild(flies,1);
    
    //4. 我方超级英雄
    MainHero = CCSprite::createWithSpriteFrameName("Miku.png");
    MainHero->setPosition(ccp(size.width/4, winSize.height/2));
    CameraLayer->addChild(MainHero,3);
    
    //5. 敌方超级英雄
    string HeroTexture[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    int scale[6] = {-1,1,-1,1,-1,1};
    BotHero = CCSprite::createWithSpriteFrameName(HeroTexture[BotIndex-1].c_str());
    BotHero->setScaleX(scale[BotIndex-1]);
    BotHero->setPosition(ccp(size.width*15/4, winSize.height/2));
    CameraLayer->addChild(BotHero,3);
      
    activeSkillNameAnimation();
}