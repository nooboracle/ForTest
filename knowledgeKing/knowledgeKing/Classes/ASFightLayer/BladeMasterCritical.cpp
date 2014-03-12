#include "ASFightLayer.h"
#include <sys/timeb.h>
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASFightLayer::BladeMasterPreAttackCritical(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bmCritical.mp3");
    
    //1.英雄身上闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -3, -198, -213);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 3, 198, 213);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.龙头
    blade = CCSprite::createWithSpriteFrameName("Blade_C_0.png");
    blade->setColor(ccc3(223,0,29));
    blade->setScale(3);
    blade->setPosition(ccp(size.width*34/70,winSize.height*34/70));
    addChild(blade,2);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 8; i++) {
        string texName = "Blade_C_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.11);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCActionInterval* fadeIn = CCFadeIn::create(1);
    CCActionInterval* dragonEffect = CCSpawn::create(fadeIn,pAnimate,NULL);
    blade->runAction(dragonEffect);
}

void ASFightLayer::BladeMasterInAttackCritical(){
    
    //1.镜头向右侧移动,并逐渐拉远
    CCActionInterval* moveOut = CCMoveBy::create(2, ccp(-size.width*4, 0));
    CameraLayer->runAction(moveOut);
    
    //2.龙头移动
    CCActionInterval* moveSlowly = CCMoveBy::create(2, ccp(-size.width/4,0));
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::BladeMasterHitEnemyCritical));
    CCSequence* hitseq = CCSequence::create(moveSlowly,hit,NULL);
    blade->runAction(hitseq);
    
    //3.刀波
    CCSprite* db = CCSprite::createWithSpriteFrameName("Blade_DB_0.png");
    db->setScale(3);
    db->setPosition(ccp(size.width*50/70,winSize.height*38/70));
    addChild(db,2);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 8; i++) {
        string texName = "Blade_DB_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.11);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCActionInterval* fadeIn = CCFadeIn::create(1);
    CCActionInterval* dragonEffect = CCSpawn::create(fadeIn,pAnimate,NULL);
    db->runAction(dragonEffect);
    
    //4.刀波移动
    CCActionInterval* moveSlowly1 = CCMoveBy::create(2, ccp(-size.width/4,0));
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCSequence* hitseq1 = CCSequence::create(moveSlowly1,remove,NULL);
    db->runAction(hitseq1);
}

void ASFightLayer::BladeMasterHitEnemyCritical(){
    
    removeChild(blade);
    
    //1.击中特效
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
    
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCCallFunc* smush = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::shadowSlash));
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,smush,NULL);
    hitEffect->runAction(seqqq);
    
    //2.英雄被击中后的动作
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq);
    
    //3.镜头开始向上移动
    CCDelayTime* delay = CCDelayTime::create(3);
    CCCallFunc* unsche = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::endShadowSlash));
    CCSequence* seqCam = CCSequence::create(delay,unsche,NULL);
    CameraLayer->runAction(seqCam);
}

void ASFightLayer::shadowSlash(){
    unschedule(schedule_selector(ASFightLayer::shadowSlash));
    
    //0.用于交换
    int tmp1[2][2] = {{0,8},{4,12}};
    string tmp2[2] = {"Blade_S_0.png","Blade_S_4.png"};
    int index = BMCounter%2;
    BMCounter++;
    
    
    //1.第一刀
    CCSprite* s1 = CCSprite::createWithSpriteFrameName(tmp2[index].c_str());
    s1->setScale(2);
    s1->setPosition(ccp(size.width*7/2,winSize.height/2));
    CameraLayer->addChild(s1,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = tmp1[index][0] ; i < tmp1[index][1]; i++) {
        string texName = "Blade_S_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.11);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCActionInterval* smush = CCMoveBy::create(0.8, ccp(size.width*2, BotHero->getPosition().y - s1->getPosition().y));
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCSequence* smushremove = CCSequence::create(pAnimate,remove,NULL);
    CCActionInterval* effect3 = CCSpawn::create(smushremove,smush,NULL);
    CCDelayTime* delay = CCDelayTime::create(0.5);
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::shadowSlashHitEnemy));
    CCSequence* seq = CCSequence::create(delay,hit,NULL);
    CCActionInterval* effect = CCSpawn::create(effect3,seq,NULL);
    s1->runAction(effect);

    //2.第二刀
    CCSprite* s2 = CCSprite::createWithSpriteFrameName(tmp2[index].c_str());
    s2->setScaleY(2);
    s2->setScaleX(-2);
    s2->setPosition(ccp(size.width*13/2,winSize.height/2));
    CameraLayer->addChild(s2,4);
    
    CCAnimation* pAnimation1 = CCAnimation::create();
    for (int i = tmp1[1-index][0] ; i < tmp1[1-index][1] ; i++) {
        string texName = "Blade_S_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation1->addSpriteFrame(frame);
    }
    pAnimation1->setDelayPerUnit(0.11);
    pAnimation1->setRestoreOriginalFrame(true);
    pAnimation1->setLoops(1);
    CCAnimate* pAnimate1 = CCAnimate::create(pAnimation1);
    
    CCDelayTime* delay1 = CCDelayTime::create(0.5);
    CCActionInterval* smush1 = CCMoveBy::create(0.8, ccp(-size.width*3, BotHero->getPosition().y - s2->getPosition().y));
    CCCallFunc* remove1 = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCSequence* smushremove1 = CCSequence::create(pAnimate1,remove1,NULL);
    CCActionInterval* effect1 = CCSpawn::create(smushremove1,smush1,NULL);
    CCDelayTime* delay2 = CCDelayTime::create(0.3);
    CCCallFunc* hit1 = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::shadowSlashHitEnemy));
    CCSequence* seq1= CCSequence::create(delay2,hit1,NULL);
    CCActionInterval* effect2 = CCSpawn::create(effect1,seq1,NULL);
    CCSequence* seq2 = CCSequence::create(delay1,effect2,NULL);
    s2->runAction(seq2);
    
    //3.循环
    schedule(schedule_selector(ASFightLayer::shadowSlash),0.4);
}

void ASFightLayer::shadowSlashHitEnemy(){
    
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq);
}

void ASFightLayer::endShadowSlash(){
    unschedule(schedule_selector(ASFightLayer::shadowSlash));
    schedule(schedule_selector(ASFightLayer::phoenixAttack),1);
}

void ASFightLayer::phoenixAttack(){
    
    unschedule(schedule_selector(ASFightLayer::phoenixAttack));
    
    //1.龙头
    CCSprite* ph = CCSprite::createWithSpriteFrameName("Blade_CH_0.png");
    ph->setScale(3);
    ph->setPosition(ccp(size.width/2,winSize.height*3/4));
    addChild(ph,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 24; i++) {
        string texName = "Blade_CH_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.11);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);

    //2. 1.5秒后触发击中动画
    CCDelayTime* delay = CCDelayTime::create(1.5);
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::phoenixMinusHP));
    CCSequence* effect1 = CCSequence::create(delay,hit,NULL);
    //3. 凤凰动画结束后移除
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCSequence* effect2 = CCSequence::create(pAnimate,remove,NULL);
    //4. 2和3同时播放，结束后播放龙的攻击动画
    CCActionInterval* effect3 = CCSpawn::create(effect1,effect2,NULL);
    CCCallFunc* dragon = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::dragonAttack));
    CCSequence* seq = CCSequence::create(effect3,dragon,NULL);
    ph->runAction(seq);
}

void ASFightLayer::phoenixMinusHP(){
    
    //2.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_hit_0.png");
    hitEffect->setScaleY(5);
    hitEffect->setScaleX(-5);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*22/30,winSize.height*100/200));
    addChild(hitEffect,4);
    
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
    
    //3.英雄被击中后的动作
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq);
    
    //4.减血提示
    string damageStr = "-" + int2string(damage/2);
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

void ASFightLayer::dragonAttack(){
    
    //1.龙头
    CCSprite* dr = CCSprite::createWithSpriteFrameName("Blade_C_0.png");
    dr->setColor(ccc3(223,0,29));
    dr->setRotation(90);
    dr->setScale(3);
    dr->setPosition(ccp(size.width/2,winSize.height*7/4));
    addChild(dr,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 8; i++) {
        string texName = "Blade_C_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.11);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCActionInterval* moveDown = CCMoveBy::create(0.5, ccp(0, -winSize.height));
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::dragonMinusHP));
    CCSequence* seq = CCSequence::create(moveDown,remove,hit,NULL);
    CCActionInterval* effect = CCSpawn::create(seq,pAnimate,NULL);
    dr->runAction(effect);
}

void ASFightLayer::dragonMinusHP(){
    
    //1.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_hit_0.png");
    hitEffect->setScaleY(5);
    hitEffect->setScaleX(-5);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*22/30,winSize.height*100/200));
    addChild(hitEffect,4);
    
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
    string damageStr = "-" + int2string(damage/2);
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

void ASFightLayer::InitBM(){
    
    BMCounter = 0;
 
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
    flies->setScale(0.417);
    flies->setPosition(ccp(size.width*11/10,winSize.height/2));
    addChild(flies,1);
    
    //4.镜头层
    CameraLayer = CCLayer::create();
    CameraLayer->setPosition(CCPointZero);
    addChild(CameraLayer,2);
    
    //4. 我方超级英雄
    string HeroTexture[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    MainHero = CCSprite::createWithSpriteFrameName(HeroTexture[MainIndex-1].c_str());
    MainHero->setPosition(ccp(size.width*5/4, winSize.height/2));
    CameraLayer->addChild(MainHero,3);
    
    CCActionInterval* moveIn = CCMoveBy::create(1, ccp(-size.width, 0));
    CCCallFunc* skillNameAnimation = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::activeSkillNameAnimation));
    CCSequence* skillSeq = CCSequence::create(moveIn,skillNameAnimation,NULL);
    MainHero->runAction(skillSeq);
    
    //5. 敌方超级英雄
    int scale[6] = {-1,1,-1,1,-1,1};
    BotHero = CCSprite::createWithSpriteFrameName(HeroTexture[BotIndex-1].c_str());
    BotHero->setScaleX(scale[BotIndex-1]);
    BotHero->setPosition(ccp(size.width*19/4, winSize.height/2));
    CameraLayer->addChild(BotHero,3);
}

