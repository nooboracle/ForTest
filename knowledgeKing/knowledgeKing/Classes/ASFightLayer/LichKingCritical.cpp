#include "ASFightLayer.h"
#include "global.h"
#include <sys/timeb.h>
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASFightLayer::LichKingPreAttackCritical(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("lkCritical.mp3", false);
    
    //1.巫妖王闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -155, -155, -155);
    CCDelayTime* delay = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 155, 155, 155);
    CCSequence* seq = CCSequence::create(flash,delay,flash1,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    MainHero->runAction(effect);
    
    //2.巫妖王的续力动画,1秒后消失
    blade = CCSprite::createWithSpriteFrameName("LichKing_C0_0.png");
    blade->setScale(5);
    blade->setPosition(ccp(size.width*3/7, winSize.height/2));
    addChild(blade,2);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 10; i++) {
        string texName = "LichKing_C0_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.14);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    CCDelayTime* delay_Remove = CCDelayTime::create(0.8);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
    CCSequence* effect_remove = CCSequence::create(delay_Remove,remove,NULL);
    CCActionInterval* effect1 = CCSpawn::create(pAnimate,effect_remove,NULL);
    blade->runAction(effect1);
}
void ASFightLayer::LichKingInAttackCritical(){
    
    //1.镜头向右侧移动
    CCActionInterval* moveOut = CCMoveBy::create(1, ccp(-size.width*4, 0));
    CameraLayer->runAction(moveOut);
    
    //2.龙飞出
    CCPoint position[3] = {ccp(size.width/3-size.width/4,winSize.height*4.4/7),ccp(size.width*2/3-size.width/4,winSize.height*3.9/7),ccp(size.width/2-size.width/4,winSize.height*3.4/7)};
    float scale[3] = {0.5,0.8,1};
    for (int i = 0 ;  i < 3 ; i ++) {
        CCSprite* dragon = CCSprite::createWithSpriteFrameName("LichKing_C2_0.png");
        dragon->setScale(3*scale[i]);
        dragon->setPosition(position[i]);
        addChild(dragon,2);
        
        CCAnimation* pAnimation = CCAnimation::create();
        for (int i = 0 ; i < 13; i++) {
            string texName = "LichKing_C2_" + int2string(i) + ".png";
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
            pAnimation->addSpriteFrame(frame);
        }
        pAnimation->setDelayPerUnit(0.15);
        pAnimation->setRestoreOriginalFrame(true);
        pAnimation->setLoops(1);
        
        CCDelayTime* delay1 = CCDelayTime::create(0.2);
        CCAnimate* pAnimate = CCAnimate::create(pAnimation);
        CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeThis));
        CCSequence* effect1 = CCSequence::create(delay1,pAnimate,remove,NULL);
        CCDelayTime* delay2 = CCDelayTime::create(0.5);
        CCActionInterval* moveBack = CCMoveBy::create(1.5, ccp(size.width, 0));
        CCSequence* effect2 = CCSequence::create(delay2,moveBack,NULL);
        CCDelayTime* delay3 = CCDelayTime::create(1);
        if (i == 0) {
            CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::LichKingHitEnemyCritical));
            CCSequence* effect3 = CCSequence::create(delay3,hit,NULL);
            CCActionInterval* effect4 = CCSpawn::create(effect2,effect1,effect3,NULL);
            dragon->runAction(effect4);
        }else{
            CCActionInterval* effect3 = CCSpawn::create(effect1,effect2,NULL);
            dragon->runAction(effect3);
        }
    }
}
void ASFightLayer::LichKingHitEnemyCritical(){
    
    //1.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_hit_0.png");
    hitEffect->setScale(3);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*4/7,winSize.height/2));
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
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,NULL);
    hitEffect->runAction(seqqq);
    
    //3.英雄被击中后的动作
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* effect_shake = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(effect_shake);
    
    //4.减血提示
    string damageStr = "-" + int2string(damage/4);
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setPosition(ccp(size.width*48/70, winSize.height*5.5/7));
    minusHP->setScale(10);
    minusHP->setRotation(20);
    addChild(minusHP,5);
    
    //4.1 血落下
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(1);
    CCSequence* effect1 = CCSequence::create(scaleDown,fadeOut,NULL);
    
    //4.2 0.5秒后开始下冰柱
    CCDelayTime* delay = CCDelayTime::create(0.5);
    CCCallFunc* cube = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::IceCubeFall));
    CCSequence* effect = CCSequence::create(delay,cube,NULL);
    
    //4.3 4.1与4.2同时播放
    CCActionInterval* effect2 = CCSpawn::create(effect,effect1,NULL);
    minusHP->runAction(effect2);
    
    //5.被击中的敌人闪光
    CCActionInterval* flash = CCTintBy::create(0.2, -255, -91, -220);
    CCDelayTime* delay1 = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 255, 91, 220);
    CCSequence* effect3 = CCSequence::create(flash,delay1,flash1,NULL);
    CCRepeatForever* effect4 = CCRepeatForever::create(effect3);
    BotHero->runAction(effect4);
}

void ASFightLayer::IceCubeFall(){
    
    CCPoint position[6] = {
        ccp(size.width/4,winSize.height),ccp(size.width*3.6/4,winSize.height),
        ccp(size.width*1.8/4,winSize.height*3.3/7 + winSize.height/2),ccp(winSize.width*2.8/4, winSize.height*3.3/7+winSize.height/2),
        ccp(size.width*2.4/4, winSize.height*3.7/7 + winSize.height/2),ccp(winSize.width*3.2/4, winSize.height*3.7/7+winSize.height/2)};
    float scale[6] = {1,1,1.2,1.2,0.8,0.8};
    int zOrder[6] = {2,2,2,2,0,0};
    for (int i = 0 ;  i < 6 ; i ++) {
        IceCube[i] = CCSprite::createWithSpriteFrameName("LichKing_C1_0.png");
        IceCube[i]->setScale(1.5*scale[i]);
        IceCube[i]->setPosition(position[i]);
        addChild(IceCube[i],zOrder[i]);
        
        CCAnimation* pAnimation = CCAnimation::create();
        for (int i = 0 ; i < 10; i++) {
            string texName = "LichKing_C1_" + int2string(i) + ".png";
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
            pAnimation->addSpriteFrame(frame);
        }
        pAnimation->setDelayPerUnit(0.15);
        pAnimation->setRestoreOriginalFrame(true);
        pAnimation->setLoops(-1);
        
        CCAnimate* pAnimate = CCAnimate::create(pAnimation);
        CCDelayTime* delay1 = CCDelayTime::create(0.5);
        CCActionInterval* moveBack = CCMoveBy::create(0.5, ccp(0,-winSize.height*12/20));
        if (i == 0) {
            CCSequence* effect1 = CCSequence::create(delay1,moveBack,NULL);
            CCDelayTime* delay2 = CCDelayTime::create(0.8);
            CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::IceCubeHit));
            CCSequence* effect2 = CCSequence::create(delay2,hit,NULL);
            CCActionInterval* effect3 = CCSpawn::create(pAnimate,effect1,effect2,NULL);
            IceCube[i]->runAction(effect3);
        }else{
            CCSequence* effect1 = CCSequence::create(delay1,moveBack,NULL);
            CCActionInterval* effect2 = CCSpawn::create(pAnimate,effect1,NULL);
            IceCube[i]->runAction(effect2);
        }
    }
}

void ASFightLayer::IceCubeHit(){
    
    //1.击中特效
    CCSprite* hitEffect = CCSprite::createWithSpriteFrameName("Sheldon_hit_0.png");
    hitEffect->setScale(3);
    hitEffect->setOpacity(200);
    hitEffect->setPosition(ccp(size.width*4/7,winSize.height/2));
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
    CCSequence* seqqq = CCSequence::create(pAnimate,remove,NULL);
    hitEffect->runAction(seqqq);
    
    //2.英雄被击中后的动作
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* effect_shake = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(effect_shake);
    
    //3.减血提示
    string damageStr = "-" + int2string(damage/4);
    minusHP = CCLabelTTF::create(damageStr.c_str(), "Arial", 62.5);
    minusHP->setScale(10);
    minusHP->setPosition(ccp(size.width*48/70, winSize.height*5.5/7));
    minusHP->setRotation(20);
    addChild(minusHP,5);
    
    //3.1 血落下
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* fadeOut = CCFadeOut::create(1);
    CCSequence* effect = CCSequence::create(scaleDown,fadeOut,NULL);
    
    //3.2 0.5秒后剑出现
    CCDelayTime* delay = CCDelayTime::create(0.5);
    CCCallFunc* sword = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::SwordFall));
    CCSequence* effect1 = CCSequence::create(delay,sword,NULL);
    
    //3.3 3.1和3.2同时播放
    CCActionInterval* effect2 = CCSpawn::create(effect,effect1,NULL);
    minusHP->runAction(effect2);
    
    //4.被击中的敌人闪
    CCActionInterval* flash = CCTintBy::create(0.2, -255, -91, -220);
    CCDelayTime* delay_flash = CCDelayTime::create(0.1);
    CCActionInterval* flash1 = CCTintBy::create(0.2, 255, 91, 220);
    CCSequence* effect3 = CCSequence::create(flash,delay_flash,flash1,NULL);
    CCRepeatForever* effect4 = CCRepeatForever::create(effect3);
    BotHero->runAction(effect4);
}

void ASFightLayer::SwordFall(){
    
    sword = CCSprite::createWithSpriteFrameName("LichKing_C3_0.png");
    sword->setScale(7);
    sword->setPosition(ccp(size.width*4/7, winSize.height*6/7));
    addChild(sword,2);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 24; i++) {
        string texName = "LichKing_C3_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.14);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    CCDelayTime* delay = CCDelayTime::create(2);
    CCActionInterval* moveDown = CCMoveBy::create(0.5, ccp(0, -winSize.height*11/20));
    CCSequence* effect1 = CCSequence::create(delay,moveDown,NULL);
    CCDelayTime* delay1 = CCDelayTime::create(2.3);
    CCCallFunc* hit = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::LichKingBigAttack));
    CCSequence* effect2 = CCSequence::create(delay1,hit,NULL);
    CCActionInterval* effect3 = CCSpawn::create(pAnimate,effect1,effect2,NULL);
    
    sword->runAction(effect3);
}

void ASFightLayer::LichKingBigAttack(){
    
    CCLayerColor* layer = CCLayerColor::create(ccc4(255,255,255,255));
    addChild(layer,5);
    CCActionInterval* fadeIn = CCFadeIn::create(0.2);
    CCDelayTime* delay = CCDelayTime::create(1);
    CCActionInterval* fadeOut = CCFadeOut::create(0.5);
    CCCallFunc* minusHP = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::LichKingMinusHP));
    CCSequence* effect2 = CCSequence::create(fadeIn,delay,fadeOut,minusHP,NULL);
    CCDelayTime* delay1 = CCDelayTime::create(1);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::removeSwordAndCube));
    CCSequence* effect1 = CCSequence::create(delay1,remove,NULL);
    CCActionInterval* effect3 = CCSpawn::create(effect1,effect2,NULL);

    layer->runAction(effect3);
    
    CCActionInterval* shake1 = CCRotateTo::create(0.15, 30);
    CCActionInterval* shake2 = CCRotateTo::create(0.15, 0);
    CCActionInterval* shake3 = CCRotateTo::create(0.15, -30);
    CCActionInterval* shake4 = CCRotateTo::create(0.15, 0);
    CCSequence* seq1 = CCSequence::create(shake1,shake2,shake3,shake4,NULL);
    BotHero->runAction(seq1);
}

void ASFightLayer::removeSwordAndCube(){
    removeChild(sword);
    removeChild(IceCube[0]);
    removeChild(IceCube[1]);
    removeChild(IceCube[2]);
    removeChild(IceCube[3]);
    removeChild(IceCube[4]);
    removeChild(IceCube[5]);
}

void ASFightLayer::LichKingMinusHP(){
    
    string damageStr = "-" + int2string(damage/2) + "\n" + "答错遭受额外伤害！";
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

void ASFightLayer::InitLK(){
    
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
    MainHero = CCSprite::createWithSpriteFrameName("LichKing.png");
    MainHero->setPosition(ccp(size.width*3/2, winSize.height/2));
    CameraLayer->addChild(MainHero,3);
    
    CCActionInterval* moveIn = CCMoveBy::create(1, ccp(-size.width, 0));
    CCCallFunc* skillNameAnimation = CCCallFuncN::create(this, callfuncN_selector(ASFightLayer::activeSkillNameAnimation));
    CCSequence* skillSeq = CCSequence::create(moveIn,skillNameAnimation,NULL);
    MainHero->runAction(skillSeq);
    
    //5. 敌方超级英雄
    string HeroTexture[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    int scale[6] = {-1,1,-1,1,-1,1};
    BotHero = CCSprite::createWithSpriteFrameName(HeroTexture[BotIndex-1].c_str());
    BotHero->setScaleX(scale[BotIndex-1]);
    BotHero->setPosition(ccp(size.width*9/2, winSize.height/2));
    CameraLayer->addChild(BotHero,3);
}