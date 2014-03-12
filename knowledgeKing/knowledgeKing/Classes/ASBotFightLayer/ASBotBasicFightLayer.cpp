#include "ASBotFightLayer.h"
#include "ASGame.h"
#include "ASBot.h"
#include "global.h"
#include <sys/timeb.h>
#include "ASUser.h"
using namespace std;

extern ASGame* player1;
extern ASBot* bot2;
extern ASUser* MainUser;

ASBotFightLayer* ASBotFightLayer::createAsFightLayer(int _MainIndex,int _BotIndex,bool _isCritical,int _damage,int _playerNumber,
                                               int _moveCount,int _shengming,int _botMoveCount,int _botShengMing){
    ASBotFightLayer* FightLayer = new ASBotFightLayer();
    if (FightLayer){
        if (_isCritical)
            FightLayer->initCriticalFightLayer(_MainIndex, _BotIndex, _isCritical,_damage,_playerNumber,_moveCount,_shengming,_botMoveCount,_botShengMing);
        else
            FightLayer->initBasicFightLayer(_MainIndex,_BotIndex,_isCritical,_damage,_playerNumber,_moveCount,_shengming,_botMoveCount,_botShengMing);
    
        return FightLayer;
    }
    return NULL;
}

void ASBotFightLayer::initBasicFightLayer(int _MainIndex,int _BotIndex,bool _isCritical,int _damage,int _playerNumber,int _moveCount,int _shengming,int _botMoveCount,int _botShengMing){
    
    MainIndex = _MainIndex;
    BotIndex = _BotIndex;
    damage = _damage;
    playerNumber = _playerNumber;
    moveCount = _moveCount;
    shengming = _shengming;
    botMoveCount = _botMoveCount;
    botShengMing = _botShengMing;
    isCritical = _isCritical;
    
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
    string HeroTexture[7] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png","James.png"};
    int scaleM[6] = {-1,1,-1,1,-1,1};
    MainHero = CCSprite::createWithSpriteFrameName(HeroTexture[MainIndex-1].c_str());
    MainHero->setScaleX(scaleM[MainIndex-1]);
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

void ASBotFightLayer::activeSkillNameAnimation(){
    
    string activeSkillNameStr[6] = {"干扰","余音","折磨","无语","嗜血","求战"};
    activeSkillName = CCLabelTTF::create(activeSkillNameStr[MainIndex-1].c_str(),"Arial",50);
    activeSkillName->setScale(30);
    activeSkillName->setPosition(ccp(size.width/4, winSize.height/2));
    CameraLayer->addChild(activeSkillName,0);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* moveSlowly = CCMoveBy::create(0.5, ccp(-size.width/100, 0));
    CCActionInterval* fadeOut = CCFadeOut::create(0.5);
    CCActionInterval* effect = CCSpawn::create(moveSlowly,fadeOut,NULL);
    CCCallFunc* passiveSkillNameEffect = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::passiveSkillNameAnimation));
    CCSequence* seq = CCSequence::create(scaleDown,effect,passiveSkillNameEffect,NULL);
    activeSkillName->runAction(seq);
}

void ASBotFightLayer::passiveSkillNameAnimation(){
    
    string passiveSkillNameStr[6] = {"超载","葱香","霜甲","天才","狂暴","小强"};
    passiveSkillName = CCLabelTTF::create(passiveSkillNameStr[MainIndex-1].c_str(), "Arial", 50);
    passiveSkillName->setScale(30);
    passiveSkillName->setPosition(ccp(size.width/4, winSize.height/2));
    CameraLayer->addChild(passiveSkillName,0);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3,1);
    CCActionInterval* moveSlowly = CCMoveBy::create(0.5, ccp(-size.width/100, 0));
    CCActionInterval* fadeOut = CCFadeOut::create(0.5);
    CCActionInterval* effect = CCSpawn::create(moveSlowly,fadeOut,NULL);
    CCCallFunc* fightAnimation;
    CCSequence* seq;
    //1.普通攻击
    if (MainIndex == 6 && !isCritical) {
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::LuffyInAttack));
        LuffyPreAttack();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 1 && !isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::IronManInAttack));
        IronManPreAttack();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 2 && !isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::MikuInAttack));
        MikuPreAttack();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 4 && !isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::SheldonInAttack));
        SheldonPreAttack();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 3 && !isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::LichKingInAttack));
        LichKingPreAttack();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 5 && !isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::BladeMasterInAttack));
        BladeMasterPreAttack();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }
    //2.暴击攻击
    if (MainIndex == 6 && isCritical) {
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::LuffyInAttack));
        LuffyPreAttack();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 1 && isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::IronManInAttackCritical));
        IronManPreAttackCritical();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 2 && isCritical){
        CCDelayTime* delay = CCDelayTime::create(0.9);
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::MikuInAttackCritical));
        CCSequence* effectMiku = CCSequence::create(delay,fightAnimation,NULL);
        MikuPreAttackCritical();
        seq = CCSequence::create(scaleDown,effect,effectMiku,NULL);
    }else if(MainIndex == 4 && isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::SheldonInAttackCritical));
        SheldonPreAttackCritical();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 3 && isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::LichKingInAttackCritical));
        LichKingPreAttackCritical();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }else if(MainIndex == 5 && isCritical){
        fightAnimation = CCCallFuncN::create(this, callfuncN_selector(ASBotFightLayer::BladeMasterInAttackCritical));
        BladeMasterPreAttackCritical();
        seq = CCSequence::create(scaleDown,effect,fightAnimation,NULL);
    }
    passiveSkillName->runAction(seq);

}

void ASBotFightLayer::showHeroInformation(){

    //1. 我方人物头像
    string HeroImageTex[6] = {"HeroFightImage_0.png","HeroFightImage_1.png","HeroFightImage_2.png","HeroFightImage_3.png","HeroFightImage_4.png","HeroFightImage_5.png"};
    MainImage = CCSprite::createWithSpriteFrameName(HeroImageTex[MainIndex-1].c_str());
    MainImage->setPosition(ccp(size.width*8/9, winSize.height*8/9));
    addChild(MainImage,100);
    
    //2. 我方人物姓名
    string mainStr = "LV." + int2string(bot2->Level) + " " + bot2->botNickName;
    CCLabelTTF* mainName = CCLabelTTF::create(mainStr.c_str(),"Arial Rounded MT Bold",30,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    mainName->setPosition(ccp(size.height*50.5/100,winSize.height*82/88));
    addChild(mainName,100);
    
    //2. hp
    string HPStr = "生命力:" + int2string(botShengMing);
    MainHP = CCLabelTTF::create(HPStr.c_str(), "Arial Rounded MT Bold", 30,CCSizeMake(size.width/3, 0),kCCTextAlignmentLeft);
    MainHP->setPosition(ccp(size.width*58.5/100, winSize.height*78/88));
    addChild(MainHP,100);
    
    //3. sp
    string SPStr = "行动力:" + int2string(botMoveCount);
    MainSP = CCLabelTTF::create(SPStr.c_str(), "Arial Rounded MT Bold", 30,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    MainSP->setPosition(ccp(size.width*66.75/100, winSize.height*74/88));
    addChild(MainSP,100);
    
    //5. 敌方人物头像
    int scale[6] = {-1,1,1,1,-1,1};
    BotImage = CCSprite::createWithSpriteFrameName(HeroImageTex[BotIndex-1].c_str());
    BotImage->setScaleX(scale[BotIndex-1]);
    BotImage->setPosition(ccp(size.width/9, winSize.height/9));
    addChild(BotImage,100);
    
    //6. 敌方人物姓名
    string botStr = "LV." + int2string(player1->Level) + " " + MainUser->nickName;
    CCLabelTTF* botName = CCLabelTTF::create(botStr.c_str(),"Arial Rounded MT Bold",30,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    botName->setPosition(ccp(size.height*36.5/100,winSize.height*14/88));
    addChild(botName,100);
    
    //7. hp
    string botHPStr = "生命力:" + int2string(shengming);
    BotHP = CCLabelTTF::create(botHPStr.c_str(), "Arial Rounded MT Bold", 30,CCSizeMake(size.width/3, 0),kCCTextAlignmentLeft);
    BotHP->setPosition(ccp(size.width*40/100, winSize.height*10/88));
    addChild(BotHP,100);
    
    //8. sp
    string botSPStr = "行动力:" + int2string(moveCount);
    BotSP = CCLabelTTF::create(botSPStr.c_str(), "Arial Rounded MT Bold", 30,CCSizeMake(size.width/3, 0),kCCTextAlignmentLeft);
    BotSP->setPosition(ccp(size.width*40/100, winSize.height*6/88));
    addChild(BotSP,100);
}

void ASBotFightLayer::changeMainHeroInformation(int _damage,int _sp){
    MainHP->setString(int2string(shengming+_damage).c_str());
    MainSP->setString(int2string(moveCount+_sp).c_str());
}

void ASBotFightLayer::changeBotHeroInformation(int _damage, int _sp){
    BotHP->setString(int2string(botShengMing+_damage).c_str());
    BotSP->setString(int2string(botMoveCount+_sp).c_str());
}

void ASBotFightLayer::backToMainGame(){
    if (playerNumber == 1)
        player1->afterFightAnimation();
    else if(playerNumber == 2)
        bot2->afterFightAnimation();
    
    getParent()->removeChild(this);
}

void ASBotFightLayer::removeThis(CCNode* sender){
    removeChild(sender,true);
}