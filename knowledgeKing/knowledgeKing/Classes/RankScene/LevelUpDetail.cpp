#include "RankScene.h"
#include "global.h"
#include "ASUser.h"
#include "AsChooseHero.h"
#include "SimpleAudioEngine.h"
extern ASUser* MainUser;

void RankScene::LevelUpHeroDetail(){
    
    disableButtons();
    
    LevelUpLayerHasBeenOpened = true;

    //1.黑色底层
    LevelUpHeroDetailLayer = CCLayerColor::create(ccc4(0,0,0,175));
    LevelUpHeroDetailLayer->setPosition(CCPointZero);
    addChild(LevelUpHeroDetailLayer,10);
    
    //2.底板
    CCSprite* table = CCSprite::createWithSpriteFrameName("LevelUpTable.png");
    table->setPosition(ccp(winSize.width/2,size.height*9.7/20+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(table,11);
    
    //3.英雄
    string roleTexture[5] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png"};
    CCSprite* hero = CCSprite::createWithSpriteFrameName(roleTexture[roleIndex-1].c_str());
    hero->setScale(0.7);
    hero->setPosition(ccp(winSize.width/3,size.height*36/50+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(hero,12);
    
    //4.英雄介绍
    string introStr[5] = {
        "与一个把自己都科技化的高富帅比拼科技是没有前途的。小心卫星！",
        "百变电音歌手，一葱易有，萌体难求，“后排甩葱的朋友，你呛到我了！”",
        "想逃离无尽的折磨？凡人，你应该远离霜之哀伤，或者……带点猫粮，喵呜",
        "Geek不擅长交流，因为连他也不知道自己天才的脑子里会蹦出什么",
        "“剑圣”的称号足以证明他的战斗能力，更可怕的是，他不要命",
    };
    CCLabelTTF* intro = CCLabelTTF::create(introStr[roleIndex-1].c_str(),"Arial Rounded MT bold",17.5,CCSizeMake(size.width/2.1,0),kCCTextAlignmentLeft);
    intro->setColor(ccc3(75,75,75));
    intro->setPosition(ccp(winSize.width/3,size.height*17/30+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(intro,12);
    
    //5.关闭按钮
    CCSprite* closeSprite = CCSprite::createWithSpriteFrameName("closeMatchTable.png");
    CCSprite* closeSprite_s = CCSprite::createWithSpriteFrameName("closeMatchTable-s.png");
    CCMenuItemSprite* closeItem = CCMenuItemSprite::create(closeSprite,closeSprite_s,this,menu_selector(RankScene::closeLevelUpHeroDetail));
    closeItem->setPosition(ccp(winSize.width*73/80,size.height*8/9+winDif*2*alpha));
    CCMenu* closeMenu = CCMenu::create(closeItem,NULL);
    closeMenu->setPosition(CCPointZero);
    LevelUpHeroDetailLayer->addChild(closeMenu,12);
    
    //6.等级信息
    string LevelStr = "LV." + int2string(MainUser->HeroLevel[roleIndex-1]);
    CCLabelTTF* LevelLabel = CCLabelTTF::create(LevelStr.c_str(), "Arial Rounded MT bold", 25,CCSizeMake(size.width/8,0),kCCTextAlignmentLeft);
    LevelLabel->setColor(ccc3(58, 145, 160));
    LevelLabel->setPosition(ccp(winSize.width*57/260, size.height*300/600+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(LevelLabel,12,2000);
    
    //7.姓名
    string nameStr[6] = {"钢蛋侠","阿葱音","呜喵王","谢耳朵","剑小圣","草帽飞"};
    CCLabelTTF* name = CCLabelTTF::create(nameStr[roleIndex-1].c_str(), "Arial Rounded MT bold", 25);
    name->setColor(ccc3(58, 145, 160));
    name->setPosition(ccp(winSize.width*9/26,size.height*300/600+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(name,12);
    
    //8.标题
    string barTitleStr[4] = {"生命力","攻击力","技能效果","天赋效果"};
    for (int i = 0 ; i < 4 ; i++) {
        CCLabelTTF* barTitle = CCLabelTTF::create(barTitleStr[i].c_str(), "Arial Rounded MT bold", 21.7,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
        barTitle->setColor(ccc3(58, 145, 160));
        barTitle->setPosition(ccp(winSize.width*10.5/30, size.height*233/600-size.height*(i-1)*8.5/120+winDif*2*alpha));
        LevelUpHeroDetailLayer->addChild(barTitle,12);
        
        CCSprite* infoTable = CCSprite::createWithSpriteFrameName("LevelTable.png");
        infoTable->setPosition(ccp(winSize.width*11/30,size.height*230.5/600-size.height*(i-1)*8.35/120+winDif*2*alpha));
        LevelUpHeroDetailLayer->addChild(infoTable,12);
        
        CCSprite* levelBarSprite = CCSprite::createWithSpriteFrameName("LevelBar.png");
        CCProgressTimer* LevelBar = CCProgressTimer::create(levelBarSprite);
        LevelBar->setPosition(ccp(winSize.width*11/30, size.height*230.5/600-size.height*(i-1)*8.35/120+winDif*2*alpha));
        LevelBar->setType(kCCProgressTimerTypeBar);
        LevelBar->setMidpoint(ccp(0, 0.5));
        LevelBar->setBarChangeRate(ccp(1, 0));
        if (i == 3) {
            if (MainUser->HeroLevel[roleIndex-1] >= 10)
                LevelBar->setPercentage((MainUser->HeroLevel[roleIndex-1]-9)*100/21.);
            else
                LevelBar->setPercentage(0);
        }else
                LevelBar->setPercentage((MainUser->HeroLevel[roleIndex-1]-1)*100/29.);
        LevelUpHeroDetailLayer->addChild(LevelBar,13,2001+i);
    }
    
    //9.升级按钮
    string LevelUpItemStr,LevelUpItemStr_s;
    if (MainUser->HeroLevel[roleIndex-1] < 30){
        LevelUpItemStr = "levelUpGold.png";
        LevelUpItemStr_s = "levelUpGold-s.png";
    }else{
        LevelUpItemStr = "LevelFull.png";
        LevelUpItemStr_s = "LevelFull.png";
    }
    CCSprite* LevelUpItemSprite = CCSprite::createWithSpriteFrameName(LevelUpItemStr.c_str());
    CCSprite* LevelUpItemSprite_s = CCSprite::createWithSpriteFrameName(LevelUpItemStr_s.c_str());
    CCMenuItemSprite* LevelUpItem = CCMenuItemSprite::create(LevelUpItemSprite, LevelUpItemSprite_s,this,menu_selector(RankScene::LevelUpHero));
    LevelUpItem->setPosition(ccp(winSize.width/2, size.height*50/600+winDif*2*alpha));
    LevelUpMenuDetail = CCMenu::create(LevelUpItem,NULL);
    LevelUpMenuDetail->setPosition(CCPointZero);
    LevelUpHeroDetailLayer->addChild(LevelUpMenuDetail,12);
    
    //9.1 升级金币
    if (MainUser->HeroLevel[roleIndex-1] < 30){
        CCLabelTTF* LevelUpGold = CCLabelTTF::create(int2string(MainUser->HeroLevelUpGold[MainUser->HeroLevel[roleIndex-1]-1]).c_str(), "Arial Rounded MT bold", 22.5,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
        LevelUpGold->setColor(ccc3(149, 70, 15));
        LevelUpGold->setPosition(ccp(size.width*140/200, size.height*52.5/600+winDif*2*alpha));
        LevelUpHeroDetailLayer->addChild(LevelUpGold,13,2006);
    }
    
    //10.金币加成
    string goldPlusRateStr = "金币 +" + int2string(MainUser->HeroLevel[roleIndex-1]) + "%";
    CCLabelTTF* goldPlusRate = CCLabelTTF::create(goldPlusRateStr.c_str(), "Arial Rounded MT bold", 21.7,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    goldPlusRate->setColor(ccc3(220, 220, 220));
    goldPlusRate->setPosition(ccp(winSize.width*14.65/30, size.height*92/600+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(goldPlusRate,12,2005);
    
    //11.下一级提示
    int heartIndex[6] = {10000,10000,10000,10000,10000,10000};
    int heartPlus[6] = {200,250,250,200,150,0};
    int attackIndex[6] = {800,600,700,900,1000,200};
    int attackPlus[6] = {20,15,25,25,20,50};
    
    //11.1 生命
    string nextHeartStr = "生命力         " + int2string((heartIndex[roleIndex-1] + MainUser->HeroLevel[roleIndex-1]*heartPlus[roleIndex-1]));
    CCLabelTTF* nextHeart = CCLabelTTF::create(nextHeartStr.c_str(), "Arial Rounded MT bold", 21.7,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    nextHeart->setColor(ccc3(58, 145, 160));
    nextHeart->setPosition(ccp(winSize.width*25.5/30, size.height*275/600+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(nextHeart,12,2007);
    
    //11.2 攻击力
    string nextAttackStr = "攻击力         " + int2string((attackIndex[roleIndex-1] + MainUser->HeroLevel[roleIndex-1]*attackPlus[roleIndex-1]));
    CCLabelTTF* nextAttack = CCLabelTTF::create(nextAttackStr.c_str(), "Arial Rounded MT bold", 21.7,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    nextAttack->setColor(ccc3(58, 145, 160));
    nextAttack->setPosition(ccp(winSize.width*25.5/30, size.height*233.25/600+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(nextAttack,12,2008);
    
    //11.3 主动技能效果
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
    CCLabelTTF* nextPositive = CCLabelTTF::create(nextPositiveStr.c_str(), "Arial Rounded MT bold",21.7,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    nextPositive->setColor(ccc3(58, 145, 160));
    nextPositive->setPosition(ccp(winSize.width*25.5/30,size.height*191.15/600+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(nextPositive,12,2009);
    
    //11.4 被动技能效果
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
    CCLabelTTF* nextNegative = CCLabelTTF::create(nextNegativeStr.c_str(), "Arial Rounded MT bold",21.7,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    if (MainUser->HeroLevel[roleIndex-1] < 10)
        nextNegative->setOpacity(0);
    nextNegative->setColor(ccc3(58, 145, 160));
    nextNegative->setPosition(ccp(winSize.width*25.5/30,size.height*149.75/600+winDif*2*alpha));
    LevelUpHeroDetailLayer->addChild(nextNegative,12,2010);
    
    //11.5 解锁天赋
    if (MainUser->HeroLevel[roleIndex-1] == 9) {
        CCLabelTTF* unlockSkill = CCLabelTTF::create("解锁天赋技能", "Arial Rounded MT bold",21.7,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
        unlockSkill->setColor(ccc3(58, 145, 160));
        unlockSkill->setPosition(ccp(winSize.width*25.5/30,size.height*102/600+winDif*2*alpha));
        LevelUpHeroDetailLayer->addChild(unlockSkill,12,2011);
    }
    
    //12.技能描述
    string skillNameStr[10][2] = {
        {"干扰:","超载:"},
        {"余音:","葱香:"},
        {"折磨:","霜甲:"},
        {"无语:","天才:"},
        {"嗜血:","狂暴:"},
    };
    string skillDescStr[10][2] = {
        {"发动攻击后，使对手遭到乌云干扰","发动攻击所需的行动力减少1点(10级解锁)"},
        {"发动攻击后，你每答对一题将回复生命，持续2道题","将受到伤害的一部分反弹给对方(10级解锁)"},
        {"发动攻击后，对手每答错一题将受到额外伤害，持续2道题","获得更高的基础生命值(10级解锁)"},
        {"发动攻击后，对手将在一段时间内无法使用道具","游戏开始时额外拥有初始行动力(10级解锁)"},
        {"每次攻击时，损失自己生命，造成额外伤害","增加大量暴击伤害(10级解锁)"},
    };
    for (int i = 0; i < 2 ; i++) {
        CCLabelTTF* skillName = CCLabelTTF::create(skillNameStr[roleIndex-1][i].c_str(), "Arial Rounded MT bold", 25);
        skillName->setColor(ccc3(58, 145, 160));
        skillName->setPosition(ccp(winSize.width*370/600, size.height*470/600 -size.height*i*70/600+winDif*2*alpha));
        LevelUpHeroDetailLayer->addChild(skillName,12);
        
        CCLabelTTF* skillDesc = CCLabelTTF::create(skillDescStr[roleIndex-1][i].c_str(), "Arial Rounded MT bold",17.5,CCSizeMake(size.width/4,0),kCCTextAlignmentLeft);
        skillDesc->setColor(ccc3(75, 75, 75));
        skillDesc->setPosition(ccp(winSize.width*480/600, size.height*465/600 -size.height*i*70/600+winDif*2*alpha));
        LevelUpHeroDetailLayer->addChild(skillDesc,12);
    }
}

void RankScene::LevelUpHero(){
    int price = MainUser->HeroLevelUpGold[MainUser->HeroLevel[roleIndex-1]-1];
    if (MainUser->HeroLevel[roleIndex-1] < 30) {
        if(MainUser->gold >= price) {
            HeroLevelUpHTTP(roleIndex-1, MainUser->HeroLevel[roleIndex-1]+1-1);
        }else{
            MessageBox = AsMessageBox::createMessageBox("您的金币不足，可以前往商店购买",2,2);
            MessageBox->setPosition(CCPointZero);
            addChild(MessageBox,100);
        }
    }
}

void RankScene::closeLevelUpHeroDetail(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
    enableButtons();
    removeChild(LevelUpHeroDetailLayer);
    LevelUpLayerHasBeenOpened = false;
}