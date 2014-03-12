#include "ASBotFightLayer.h"
#include <sys/timeb.h>
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASBotFightLayer::IronManPreAttackCritical(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("ironmancritical.mp3");
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.天上的卫星1
    CCSprite* satelite1 = CCSprite::createWithSpriteFrameName("weixing.png");
    satelite1->setScale(2);
    satelite1->setPosition(ccp(size.width/2,size.height*7/2+winDif*2*alpha));
    CameraLayer->addChild(satelite1,3);
    
    //3.天上的卫星2
    CCSprite* satelite2 = CCSprite::createWithSpriteFrameName("weixing.png");
    satelite2->setScale(2);
    satelite2->setPosition(ccp(-size.width*7/4,size.height*7/2+winDif*2*alpha));
    CameraLayer->addChild(satelite2,3);
}

void ASBotFightLayer::IronManInAttackCritical(){
    
    //1.光柱1
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("IronMan_CP_0.png");
    hitEffect->setScale(3);
    hitEffect->setPosition(ccp(size.width/2,size.height*62/70+winDif*2*alpha));
    CameraLayer->addChild(hitEffect,2);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 23; i++) {
        string texName = "IronMan_CP_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.15);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::removeThis));
    CCSequence* seq = CCSequence::create(pAnimate,remove,NULL);
    CCDelayTime* delay = CCDelayTime::create(1.05);
    CCCallFunc* Emit = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::EmitLight));
    CCSequence* seq1 = CCSequence::create(delay,Emit,NULL);
    CCActionInterval* effect = CCSpawn::create(seq,seq1,NULL);
    hitEffect->runAction(effect);
}

void ASBotFightLayer::EmitLight(CCNode* sender){
    
    //1.镜头向上侧移动
    CCActionInterval* moveUp = CCMoveBy::create(1.5, ccp(0,-size.height*3));
    CCCallFunc* reach = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::reachFirstSatelite));
    CCSequence* seq = CCSequence::create(moveUp,reach,NULL);
    CameraLayer->runAction(seq);
    
    //2.光柱起点向下移动到屏幕下方边界
    sender->setScaleY(4.5);
    CCActionInterval* moveUp1 = CCMoveBy::create(1.5, ccp(0,1.75*size.height));
    CCDelayTime* delay = CCDelayTime::create(1);
    CCCallFunc* second = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::EmitSecondLight));
    CCSequence* seq1 = CCSequence::create(delay,second,NULL);
    CCActionInterval* effect = CCSpawn::create(moveUp1,seq1,NULL);
    sender->runAction(effect);
}

void ASBotFightLayer::EmitSecondLight(CCNode *sender){
    
    //1.光柱2
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("IronMan_CP_0.png");
    hitEffect->setRotation(-90);
    hitEffect->setScaleX(-2.95);
    hitEffect->setScaleY(2.95);
    hitEffect->setPosition(ccp(-size.width*0.07,size.height*7/2+winDif*2*alpha));
    CameraLayer->addChild(hitEffect,2);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 23; i++) {
        string texName = "IronMan_CP_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::removeThis));
    CCSequence* seq = CCSequence::create(pAnimate,remove,NULL);
    CCDelayTime* delay = CCDelayTime::create(1.2);
    CCActionInterval* moveRight = CCMoveBy::create(1.0, ccp(-size.width*10.4/10, 0));
    CCSequence* seq1 = CCSequence::create(delay,moveRight,NULL);
    CCActionInterval* effect = CCSpawn::create(seq,seq1,NULL);

    hitEffect->runAction(effect);
}

void ASBotFightLayer::reachFirstSatelite(){
    
    //1.镜头向右侧移动
    CCActionInterval* moveUp = CCMoveBy::create(1.8, ccp(size.width*2 + size.width/4,0));
    CCCallFunc* reach = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::reachSecondSatelite));
    CCSequence* seq = CCSequence::create(moveUp,reach,NULL);
    CameraLayer->runAction(seq);
}

void ASBotFightLayer::reachSecondSatelite(){
    
    CCDelayTime* delay = CCDelayTime::create(1.1);
    CCActionInterval* moveDown = CCMoveBy::create(1, ccp(0,size.height*3));
    CCSequence* seq = CCSequence::create(delay,moveDown,NULL);
    CCCallFunc* reach = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::IronManBigAttack));
    CCActionInterval* effect = CCSpawn::create(seq,reach,NULL);
    CameraLayer->runAction(effect);
}

void ASBotFightLayer::IronManBigAttack(){
    
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("IronMan_CHit_1.png");
    hitEffect->setScale(10);
    hitEffect->setPosition(ccp(-size.width*7/4 , size.height*5.1/2+winDif*2*alpha));
    CameraLayer->addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 1 ; i < 24; i++) {
        string texName = "IronMan_CHit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.14);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::removeThis));
    CCSequence* seq = CCSequence::create(pAnimate,remove,NULL);
    
    CCDelayTime* delay = CCDelayTime::create(1.2);
    CCActionInterval* moveDown = CCMoveBy::create(1, ccp(0, -size.height*1.45));
    CCCallFunc* flash = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::IronManHitEnemyCritical));
    CCSequence* seq1 = CCSequence::create(delay,moveDown,flash,NULL);
    
    CCActionInterval* effect = CCSpawn::create(seq,seq1,NULL);
    
    hitEffect->runAction(effect);
}

void ASBotFightLayer::IronManHitEnemyCritical(){
    
    CCLayerColor* layer = CCLayerColor::create(ccc4(255,255,255,255));
    addChild(layer,5);
    CCActionInterval* fadeIn = CCFadeIn::create(0.3);
    CCDelayTime* delay = CCDelayTime::create(1);
    CCActionInterval* fadeOut = CCFadeOut::create(0.5);
    CCCallFunc* minusHP = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::MinusHP));
    CCSequence* seq = CCSequence::create(fadeIn,delay,fadeOut,minusHP,NULL);
    layer->runAction(seq);
    
    
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq1 = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq1);
}

void ASBotFightLayer::MinusHP(){
    
    CameraLayer->removeChild(BotHero);
    
    string damageStr = "-" + int2string(damage) + "\n" + "乌云袭击！";
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 75);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*22/70, size.height*5/7+winDif*2*alpha));
    minusHP->setRotation(-20);
    addChild(minusHP,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCCallFunc* back = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::backToMainGame));
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,back,NULL);
    minusHP->runAction(seq1);
}

void ASBotFightLayer::InitIM(){
    
    //1. 战斗大背景
    fightBG = CCSprite::createWithSpriteFrameName("bg_l.png");
    fightBG->setPosition(ccp(size.width/2, winSize.height/2));
    addChild(fightBG,-1);
    
    //2.镜头层
    CameraLayer = CCLayer::create();
    CameraLayer->setPosition(CCPointZero);
    addChild(CameraLayer,2);
    
    //3. 光晕条及动画
    halo = CCSprite::createWithSpriteFrameName("superBgCover.png");
    halo->setPosition(ccp(size.width/2, winSize.height/2));
    CameraLayer->addChild(halo,2);
    
    CCActionInterval* scaleUp = CCScaleTo::create(0.3, 1.1);
    CCRepeatForever* repeat_halo = CCRepeatForever::create(scaleUp);
    halo->runAction(repeat_halo);
    
    //3. 粒子：飞翔的线条
    flies = CCParticleSystemQuad::create("fliesUp.plist");
    flies->setScale(0.417);
    flies->setPosition(ccp(size.width/2,size.height*11/10+winDif*2));
    addChild(flies,1);
    
    //4. 我方超级英雄
    string HeroTexture[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    MainHero = CCSprite::createWithSpriteFrameName(HeroTexture[MainIndex-1].c_str());
    MainHero->setScaleX(-1);
    MainHero->setPosition(ccp(size.width*3/4,winSize.height/2));
    CameraLayer->addChild(MainHero,3);
    
    //5. 敌方超级英雄
    int scale[6] = {1,1,1,-1,1,1};
    BotHero = CCSprite::createWithSpriteFrameName(HeroTexture[BotIndex-1].c_str());
    BotHero->setScaleX(scale[BotIndex-1]);
    BotHero->setPosition(ccp(-size.width*7/4, winSize.height/2));
    CameraLayer->addChild(BotHero,3);
    
    activeSkillNameAnimation();
}


