#include "ASFightLayer.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;
extern ASUser* MainUser;

void ASFightLayer::SheldonPreAttack(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sheldonattack1.mp3", false);
    
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.小老鼠
    CCPoint mousePosition[3] = {ccp(size.width/2,winSize.height/2),ccp(size.width*9/20, winSize.height*9/20),ccp(size.width*8.5/20, winSize.height*11/20)};
    for (int i = 0 ; i < 3 ; i++) {
        mouse[i] = CCSprite::createWithSpriteFrameName("Sheldon_0.png");
        mouse[i]->setScale(2);
        mouse[i]->setPosition(mousePosition[i]);
        addChild(mouse[i],2);
        
        CCAnimation* pAnimation = CCAnimation::create();
        for (int i = 0 ; i < 10; i++) {
            string texName = "Sheldon_" + int2string(i) + ".png";
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
            pAnimation->addSpriteFrame(frame);
        }
        pAnimation->setDelayPerUnit(0.14);
        pAnimation->setRestoreOriginalFrame(true);
        pAnimation->setLoops(-1);
        CCAnimate* pAnimate = CCAnimate::create(pAnimation);
        
        mouse[i]->runAction(pAnimate);
    }
}

void ASFightLayer::SheldonInAttack(){
   
    //1.镜头向右侧移动
    CCActionInterval* moveOut = CCMoveBy::create(2, ccp(-size.width*3, 0));
    CameraLayer->runAction(moveOut);

    //2.
    removeChild(mouse[0]);
    removeChild(mouse[1]);
    removeChild(mouse[2]);
    CCPoint mousePosition[3] = {ccp(size.width/2,winSize.height/2),ccp(size.width*9/20, winSize.height*9/20),ccp(size.width*8.5/20, winSize.height*11/20)};    for (int i = 0 ; i < 3 ; i++) {
        mouse[i] = CCSprite::createWithSpriteFrameName("Sheldon_10.png");
        mouse[i]->setScale(2);
        mouse[i]->setColor(ccc3(101, 176, 252));
        mouse[i]->setPosition(mousePosition[i]);
        addChild(mouse[i],2);
        
        CCAnimation* pAnimation = CCAnimation::create();
        for (int i = 10 ; i < 20; i++) {
            string texName = "Sheldon_" + int2string(i) + ".png";
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
            pAnimation->addSpriteFrame(frame);
        }
        pAnimation->setDelayPerUnit(0.1);
        pAnimation->setRestoreOriginalFrame(true);
        pAnimation->setLoops(2);
        
        CCAnimate* pAnimate = CCAnimate::create(pAnimation);
        CCActionInterval* moveBack = CCMoveBy::create(2, ccp(-size.width/4,0));
        CCActionInterval* effect1 = CCSpawn::create(pAnimate,moveBack,NULL);
        CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
        CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::MouseStopMoving));
        CCSequence* seq = CCSequence::create(effect1,remove,hit,NULL);
        mouse[i]->runAction(seq);
    }
}

void ASFightLayer::MouseStopMoving(CCNode* sender){

    removeChild(mouse[0]);
    removeChild(mouse[1]);
    removeChild(mouse[2]);
    
    CCPoint mousePosition[3] = {ccp(size.width/2-size.width/4,winSize.height/2),ccp(size.width*9/20-size.width/4, winSize.height*9/20),ccp(size.width*8.5/20-size.width/4, winSize.height*11/20)};    for (int i = 0 ; i < 3 ; i++) {
        mouse[i] = CCSprite::createWithSpriteFrameName("Sheldon_0.png");
        mouse[i]->setScale(2);
        mouse[i]->setColor(ccc3(101, 176, 252));
        mouse[i]->setPosition(mousePosition[i]);
        addChild(mouse[i],2);
        
        CCAnimation* pAnimation = CCAnimation::create();
        for (int i = 0 ; i < 10; i++) {
            string texName = "Sheldon_" + int2string(i) + ".png";
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
            pAnimation->addSpriteFrame(frame);
        }
        pAnimation->setDelayPerUnit(0.1);
        pAnimation->setRestoreOriginalFrame(true);
        pAnimation->setLoops(2);
        
        CCAnimate* pAnimate = CCAnimate::create(pAnimation);
        if (i == 0) {
            CCDelayTime* delay = CCDelayTime::create(0.5);
            CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::Bazinga));
            CCSequence* effect1 = CCSequence::create(delay,hit,NULL);
            CCActionInterval* effect2 = CCSpawn::create(pAnimate,effect1,NULL);
            mouse[i]->runAction(effect2);
        }else
            mouse[i]->runAction(pAnimate);
    }
}

void ASFightLayer::Bazinga(){
 
    string Str = "Bazinga~~";
    CCLabelTTF* Bazinga = CCLabelTTF::create(Str.c_str(), "Arial", 62.5);
    Bazinga->setScale(10);
    Bazinga->setPosition(ccp(size.width/2, winSize.height/2));
    Bazinga->setRotation(20);
    addChild(Bazinga,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.7,1);
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::SheldonHitEnemy));
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCActionInterval* effect1 = CCSpawn::create(hit,fadeOut,NULL);
    CCSequence* effect2 = CCSequence::create(scaleDown,effect1,NULL);
    Bazinga->runAction(effect2);
}

void ASFightLayer::SheldonHitEnemy(){
    
    removeChild(mouse[0]);
    removeChild(mouse[1]);
    removeChild(mouse[2]);
    
    //1.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_hit_0.png");
    hitEffect->setScale(6);
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
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCCallFunc* silent = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::SilentAnimation));
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
    string damageStr = "-" + int2string(damage);
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*48/70, winSize.height*5/7));
    minusHP->setRotation(20);
    addChild(minusHP,5);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(2);
    CCSequence* seq1 = CCSequence::create(scaleDown,fadeOut,NULL);
    minusHP->runAction(seq1);
}

void ASFightLayer::SilentAnimation(){
    
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_S_0.png");
    hitEffect->setScale(3.5);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*2/3,winSize.height/2));
    addChild(hitEffect,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 29; i++) {
        string texName = "Sheldon_S_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.07);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCCallFunc* back = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::backToMainGame));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,back,NULL);
    hitEffect->runAction(seqqq);
    
}