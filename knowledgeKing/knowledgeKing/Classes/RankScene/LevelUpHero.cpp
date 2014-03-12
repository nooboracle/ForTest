#include "RankScene.h"
#include "ASUser.h"
#include "global.h"
#include "AsChooseHero.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

extern ASUser* MainUser;

void RankScene::openLevelUpHeroLayer(CCNode* sender){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    int tmp = ((CCMenuItemSprite*)sender)->getTag() - 1010;
    //1. 解锁英雄
    if (!MainUser->HeroUnLocked[tmp]) {
        if (MainUser->gold >= MainUser->HeroUnLockPrice[tmp]) {
            HeroProcessing = tmp;
            unlockHeroHTTP(HeroProcessing);
        }
        else{
            MessageBox = AsMessageBox::createMessageBox("您的金币不足，可以前往商店购买",2,2);
            MessageBox->setPosition(CCPointZero);
            addChild(MessageBox,100);
        }
    }
    //2. 升级英雄
    else
        LevelUpHeroDetail();
}

void RankScene::changeGold(int _gold){
    MainUser->gold += _gold;
    CCLabelTTF* goldNumber = (CCLabelTTF*)this->getChildByTag(8);
    goldNumber->setString(int2string(MainUser->gold).c_str());
}

void RankScene::UnlockAnimation(int _index){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("LevelUp.wav");
    
    //1.动画
    CCSprite* ani = CCSprite::createWithSpriteFrameName("hotTriggered_0.png");
    ani->setScale(2);
    ani->setPosition(ccp(size.width*8.9/30+size.width*_index,size.height*9.3/40+winDif*2));
    roleScrollLayer->addChild(ani,4);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 15; i++) {
        string texName = "hotTriggered_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    
    CCCallFunc* remove = CCCallFuncN::create(roleScrollLayer, callfuncN_selector(RankScene::removeThis));
    CCSequence* seq = CCSequence::create(pAnimate,remove,NULL);
    ani->runAction(seq);
    
    //2.替换成升级按钮
    roleScrollLayer->removeChildByTag(1010+_index);
    roleScrollLayer->removeChildByTag(1210+_index);
    CCSprite* LevelUpItemSprite = CCSprite::createWithSpriteFrameName("levelUpButton.png");
    CCSprite* LevelUpItemSprite_s = CCSprite::createWithSpriteFrameName("levelUpButton-s.png");
    CCMenuItemSprite* LevelUpItem = CCMenuItemSprite::create(LevelUpItemSprite,LevelUpItemSprite_s,this,menu_selector(RankScene::openLevelUpHeroLayer));
    LevelUpItem->setPosition(ccp(size.width*17/26 + size.width*_index, size.height*95/600+winDif*alpha));
    LevelUpMenu->addChild(LevelUpItem,NULL);
    LevelUpItem->setTag(1010+_index);
    LevelUpMenu->setPosition(CCPointZero);
    
    //3.锁去掉
    CCActionInterval* fadeOut = CCFadeOut::create(1.5);
    CCSprite* lock = (CCSprite*)roleScrollLayer->getChildByTag(1410+_index);
    CCCallFunc* remove1 = CCCallFuncN::create(this, callfuncN_selector(RankScene::removeThis));
    CCSequence* lockSeq = CCSequence::create(fadeOut,remove1,NULL);
    lock->runAction(lockSeq);
}

void RankScene::LevelUpAnimation(int _index){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("LevelUp.wav");
    
    //1.动画
    CCSprite* LevelUp = CCSprite::createWithSpriteFrameName("LevelUp.png");
    LevelUp->setPosition(ccp(size.width/3,size.height*2/3+winDif*2));
    LevelUpHeroDetailLayer->addChild(LevelUp,13);
    
    CCActionInterval* fadeIn = CCFadeIn::create(0.5);
    CCActionInterval* JumpUp = CCJumpBy::create(0.7,ccp(0,0),20,2);
    CCActionInterval* effect = CCSpawn::create(fadeIn,JumpUp,NULL);
    CCActionInterval* fadeOut = CCFadeOut::create(0.3);
    CCCallFunc* remove = CCCallFuncN::create(this,callfuncN_selector(RankScene::removeThis));
    CCSequence* seq = CCSequence::create(effect,fadeOut,remove,NULL);
    LevelUp->runAction(seq);
    
    //2.改变等级文字
    string LevelStr = "LV." + int2string(MainUser->HeroLevel[_index]);
    CCLabelTTF* levelLabel = (CCLabelTTF*)roleScrollLayer->getChildByTag(1110+_index);
    levelLabel->setString(LevelStr.c_str());
    CCLabelTTF* levelLabelDetail = (CCLabelTTF*)LevelUpHeroDetailLayer->getChildByTag(2000);
    levelLabelDetail->setString(LevelStr.c_str());
    
    int heartIndex[6] = {10000,10000,10000,10000,10000,10000};
    int attackIndex[6] = {800,600,700,900,1000,200};
    int heartPlus[6] = {200,250,250,200,150,0};
    int attackPlus[6] = {20,15,25,25,20,50};

    //3. 改变等级进度条
    //3.1 选人面板上的等级条
    CCProgressTimer* LevelBar = (CCProgressTimer*)roleScrollLayer->getChildByTag(1310+_index);
    CCProgressFromTo* action = CCProgressFromTo::create(0.5, (MainUser->HeroLevel[_index]-2)*100/29., (MainUser->HeroLevel[_index]-1)*100/29.);
    LevelBar->runAction(action);
    //3.2 生命力条
    CCProgressTimer* LifeBar = (CCProgressTimer*)LevelUpHeroDetailLayer->getChildByTag(2001);
    CCProgressFromTo* LifeAction = CCProgressFromTo::create(0.5,(MainUser->HeroLevel[_index]-2)*100/29.,(MainUser->HeroLevel[_index]-1)*100/29.);
    LifeBar->runAction(LifeAction);
    //3.3 攻击力条
    CCProgressTimer* attackBar = (CCProgressTimer*)LevelUpHeroDetailLayer->getChildByTag(2002);
    CCProgressFromTo* attackAction = CCProgressFromTo::create(0.5, (MainUser->HeroLevel[_index]-2)*100/29.,(MainUser->HeroLevel[_index]-1)*100/29.);
    attackBar->runAction(attackAction);
    //3.4 主动技能条
    CCProgressTimer* positiveSkillBar = (CCProgressTimer*)LevelUpHeroDetailLayer->getChildByTag(2003);
    CCProgressFromTo* positiveSkillAction = CCProgressFromTo::create(0.5, (MainUser->HeroLevel[_index]-2)*100/29., (MainUser->HeroLevel[_index]-1)*100/29.);
    positiveSkillBar->runAction(positiveSkillAction);
    //3.5 天赋技能条
    if (MainUser->HeroLevel[roleIndex-1] >= 10) {
        CCProgressTimer* negativeSkillBar = (CCProgressTimer*)LevelUpHeroDetailLayer->getChildByTag(2004);
        CCProgressFromTo* negativeSkillAction = CCProgressFromTo::create(0.5, (MainUser->HeroLevel[_index]-10)*100/21.,(MainUser->HeroLevel[_index]-9)*100/21.);
        negativeSkillBar->runAction(negativeSkillAction);
    }
    
    //4.金币加成
    string goldPlusRateStr = "金币 +" + int2string(MainUser->HeroLevel[roleIndex-1]) + "%";
    CCLabelTTF* goldPlusRate = (CCLabelTTF*)LevelUpHeroDetailLayer->getChildByTag(2005);
    goldPlusRate->setString(goldPlusRateStr.c_str());
    
    //5.下一级
    //5.1 生命
    string nextHeartStr = "生命力         " + int2string((heartIndex[roleIndex-1] + MainUser->HeroLevel[roleIndex-1]*heartPlus[roleIndex-1]));
    CCLabelTTF* nextHeart = (CCLabelTTF*)LevelUpHeroDetailLayer->getChildByTag(2007);
    nextHeart->setString(nextHeartStr.c_str());
    
    //5.2 攻击力
    string nextAttackStr = "攻击力         " + int2string((attackIndex[roleIndex-1] + MainUser->HeroLevel[roleIndex-1]*attackPlus[roleIndex-1]));
    CCLabelTTF* nextAttack = (CCLabelTTF*)LevelUpHeroDetailLayer->getChildByTag(2008);
    nextAttack->setString(nextAttackStr.c_str());
    
    //5.3 主动技能效果
    string nextPositiveStr;
    if (roleIndex == 1)
        nextPositiveStr = "乌云时长      " + int2string(10 + 0.2 * (MainUser->HeroLevel[roleIndex-1] - 1)) + "秒";
    else if(roleIndex == 2)
        nextPositiveStr = "每题回复      " + int2string(500+20*(MainUser->HeroLevel[roleIndex-1]-1));
    else if(roleIndex == 3)
        nextPositiveStr = "每题伤害      " + int2string(1000+33*(MainUser->HeroLevel[roleIndex-1]-1));
    else if(roleIndex == 4)
        nextPositiveStr = "沉默时长      " + int2string(20 + 0.5*(MainUser->HeroLevel[roleIndex-1]-1));
    else if(roleIndex == 5)
        nextPositiveStr = "额外伤害      +" + int2string( (attackIndex[roleIndex-1] + MainUser->HeroLevel[roleIndex-1]*attackPlus[roleIndex-1])*(0.1 + (MainUser->HeroLevel[roleIndex-1]-1)*0.005) ) + "\n自残生命      +"
        + int2string((600-10*(MainUser->HeroLevel[roleIndex-1]-1)));
    CCLabelTTF* nextPositive = (CCLabelTTF*)LevelUpHeroDetailLayer->getChildByTag(2009);
    nextPositive->setString(nextPositiveStr.c_str());
    
    //11.4 被动技能效果
    if (MainUser->HeroLevel[roleIndex-1] > 9){
        LevelUpHeroDetailLayer->removeChildByTag(2011);
        string nextNegativeStr;
        if (roleIndex == 1)
            nextNegativeStr = "行动力基数    " + int2string(4);
        else if(roleIndex == 2)
            nextNegativeStr = "反弹伤害      " + int2string((0.2+0.015*(MainUser->HeroLevel[roleIndex-1]-10))*100) + "%";

        else if(roleIndex == 3)
            nextNegativeStr = "基础生命      " + int2string((heartIndex[roleIndex-1] + MainUser->HeroLevel[roleIndex-1]*heartPlus[roleIndex-1])*(0.15+0.01*(MainUser->HeroLevel[roleIndex-1])));
        else if(roleIndex == 4)
            nextNegativeStr = "额外行动力    " + int2string(3 + 0.1*(MainUser->HeroLevel[roleIndex-1]-10));
        else if(roleIndex == 5)
            nextNegativeStr = "暴击伤害      +" + int2string((0.2+0.015*(MainUser->HeroLevel[roleIndex-1]-10))*100) + "%";
        CCLabelTTF* nextNegative = (CCLabelTTF*)LevelUpHeroDetailLayer->getChildByTag(2010);
        nextNegative->setOpacity(255);
        nextNegative->setString(nextNegativeStr.c_str());
    }
    
    //11.5 升级金币
    if (MainUser->HeroLevel[roleIndex-1] < 30) {
        CCLabelTTF* levelUpGold = (CCLabelTTF*)LevelUpHeroDetailLayer->getChildByTag(2006);
        levelUpGold->setString(int2string(MainUser->HeroLevelUpGold[MainUser->HeroLevel[roleIndex-1]-1]).c_str());
    }else
        LevelUpHeroDetailLayer->removeChildByTag(2006);
    
    //5.如果已经满级，则将按钮禁用
    if(MainUser->HeroLevel[_index] == 30){
        LevelUpMenu->setTouchEnabled(false);
        LevelUpMenuDetail->setTouchEnabled(false);
        CCSprite* LevelUpSprite = CCSprite::createWithSpriteFrameName("LevelFull.png");
        ((CCMenuItemSprite*)((LevelUpMenuDetail->getChildren())->objectAtIndex(0)))->setNormalImage((CCNode*)LevelUpSprite);
    }
}