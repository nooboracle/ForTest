#include "ASBotFightLayer.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;
extern ASUser* MainUser;

void ASBotFightLayer::MikuPreAttack(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("mikuattack.mp3");
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.蓄力动画
    blade = CCSprite::createWithSpriteFrameName("miku_0_0.png");
    blade->setScaleX(-1.5);
    blade->setScaleY(1.5);
    blade->setPosition(ccp(size.width*37/50,winSize.height*9.5/20));
    addChild(blade,3);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 24; i++) {
        string texName = "miku_0_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.05);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::removeThis));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,NULL);
    blade->runAction(seqqq);
}

void ASBotFightLayer::MikuInAttack(){
    
    //1.镜头向右侧移动
    CCActionInterval* moveOut = CCMoveBy::create(2, ccp(size.width*3, 0));
    CameraLayer->runAction(moveOut);
    
    //2.葱
    CCSprite* cong1 = CCSprite::createWithSpriteFrameName("cong.png");
    cong1->setScaleX(-1);
    cong1->setScaleY(1);
    cong1->setPosition(ccp(size.width/2,winSize.height/2));
    addChild(cong1,2);
    
    CCSprite* cong2 = CCSprite::createWithSpriteFrameName("cong.png");
    cong2->setScaleX(-1);
    cong2->setScaleY(1);
    cong2->setPosition(ccp(size.width*11/20,winSize.height*9/20));
    addChild(cong2,2);
    
    CCSprite* cong3 = CCSprite::createWithSpriteFrameName("cong.png");
    cong3->setScaleX(-1);
    cong3->setScaleY(1);
    cong3->setPosition(ccp(size.width*11.5/20,winSize.height*11/20));
    addChild(cong3,2);
    
    //2.粒子移动
    CCActionInterval* moveBack1 = CCMoveBy::create(0.3, ccp(size.width/4, 0));
    CCActionInterval* moveSlowly1 = CCMoveBy::create(0.85, ccp(size.width*3/10 - size.width/4,0));
    CCActionInterval* moveSlowly2 = CCMoveBy::create(0.85, ccp(-size.width/13 , 0));
    CCCallFunc* hit1 = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::MikuHitEnemy));
    CCSequence* hitseq1 = CCSequence::create(moveBack1,moveSlowly1,moveSlowly2,hit1,NULL);
    cong1->runAction(hitseq1);
    
    CCActionInterval* moveBack2 = CCMoveBy::create(0.3, ccp(size.width/4, 0));
    CCActionInterval* moveSlowly3 = CCMoveBy::create(0.85, ccp(size.width/20 - size.width/4,0));
    CCActionInterval* moveSlowly4 = CCMoveBy::create(0.85, ccp(-size.width/12,0));
    CCCallFunc* hit2 = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::MikuHitEnemy));
    CCSequence* hitseq2 = CCSequence::create(moveBack2,moveSlowly3,moveSlowly4,hit2,NULL);
    cong2->runAction(hitseq2);
    
    CCActionInterval* moveBack3 = CCMoveBy::create(0.3, ccp(size.width/4, 0));
    CCActionInterval* moveSlowly5 = CCMoveBy::create(0.85, ccp(size.width/17 - size.width/4,0));
    CCActionInterval* moveSlowly6 = CCMoveBy::create(0.85, ccp(-size.width/10,0));
    CCCallFunc* hit3 = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::MikuHitEnemy));
    CCSequence* hitseq3 = CCSequence::create(moveBack3,moveSlowly5,moveSlowly6,hit3,NULL);
    cong3->runAction(hitseq3);
}

void ASBotFightLayer::MikuHitEnemy(CCNode* sender){
    
    removeChild(sender);
    
    //2.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("miku_hit_0.png");
    hitEffect->setScaleX(-1.5);
    hitEffect->setScaleY(1.5);
    hitEffect->setPosition(ccp(sender->getPosition().x-size.width*3/10, sender->getPosition().y));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 19; i++) {
        string texName = "miku_hit_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
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