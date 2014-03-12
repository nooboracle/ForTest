#include "GameBaseClass.h"
#include "LevelSelectionScene.h"
#include "AsChooseHero.h"
#include "SimpleAudioEngine.h"
#include "IOSBridge.h"
#include "ASGame.h"
#include "ASBot.h"
#include "global.h"
#include "ASUser.h"
using namespace std;

ASGame* player1;
extern ASBot* bot2;
extern ASUser* MainUser;
extern string tibaoName[100];
extern int tibaoType;
extern string imageToRemove[1000];

void GameBaseClass::CongScene(){    //0是赢 1是输 2是平局
    
    if(!MainUser->muted){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    }
    
    IOSBridge::chapingAD();
    removeChildByTag(0);
    removeChild(touchToGoOn,true);
    
    /*************1.Rank*************/
    if (isIntroducing)
        MainNewRank = 0;
    /*************2.历史累计胜场*************/
    if (BotWinTimesHistory == "")
        BotWinTimesHistory = "0";
    /*************2.本次答题正确率*************/
    if (player1->rightNumber + player1->wrongNumber == 0)
        MainRightRate = 0;
    else
        MainRightRate = (float)(player1->rightNumber/(float)(player1->rightNumber + player1->wrongNumber));
    if (bot2->rightNumber + bot2->wrongNumber == 0)
        BotRightRate = 0;
    else
        BotRightRate = (float)(bot2->rightNumber/(float)(bot2->rightNumber + bot2->wrongNumber));
    /*************3.本次最大连对*************/
    MainMaxCombo = player1->MaxCombo;
    BotMaxCombo = bot2->MaxCombo;
    
    //5.回答正确文字、播放音频
    if(!MainUser->muted){
        string audioEffect[3] = {"success.mp3","fail.wav","success.mp3"};
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(audioEffect[userSuccess].c_str(), false);
    }
    
    //5.UI部分
    //5.1 背景
    string bgTexture[2] = {"bg_success.png","bg_fail.png"};
    CCSprite* bg = CCSprite::createWithSpriteFrameName(bgTexture[userSuccess].c_str());
    bg->setPosition(ccp(size.width/2,winSize.height/2));
    addChild(bg,4);
    
    //5.2 题包名称
    CCSprite* titlePanel = CCSprite::createWithSpriteFrameName("title_result.png");
    titlePanel->setPosition(ccp(size.width/2,size.height*10.05/10+winDif*2*alpha));
    addChild(titlePanel,5);
    
    string titleStr = tibaoName[tibaoType];
    if (isIntroducing)
        titleStr = "教学模式";
    CCLabelBMFont* tibaoLabel = CCLabelBMFont::create(titleStr.c_str(),"puzzlename.fnt");
    tibaoLabel->setScale(0.85*0.417);
    tibaoLabel->setPosition(ccp(size.width/2,size.height*9.2/10+winDif*2*alpha));
    addChild(tibaoLabel,6);
    
    //5.3 底盘
    CCSprite* panel = CCSprite::createWithSpriteFrameName("panel_result.png");
    panel->setPosition(ccp(size.width/2,size.height*45/100+winDif*alpha));
    addChild(panel,6);
    
    //5.4 统计数据
    CCLabelTTF* _MainName = CCLabelTTF::create(MainUser->nickName.c_str(), "Arial Rounded MT bold", 25);
    CCLabelTTF* _botName = CCLabelTTF::create(bot2->botNickName.c_str(), "Arial Rounded MT bold", 25);
    CCLabelTTF* _MainRank = CCLabelTTF::create(int2string(MainNewRank).c_str(),"Arial Rounded MT bold",30,CCSizeMake(size.width/8, 0),kCCTextAlignmentRight);
    CCLabelTTF* _BotRank = CCLabelTTF::create(int2string(BotNewRank).c_str(),"Arial Rounded MT bold",30,CCSizeMake(size.width/8, 0),kCCTextAlignmentRight);
    CCLabelTTF* _MainWin = CCLabelTTF::create(int2string(MainUser->WinTimes[tibaoType]).c_str(),"Arial Rounded MT bold",30,CCSizeMake(size.width/8, 0),kCCTextAlignmentRight);
    CCLabelTTF* _BotWin = CCLabelTTF::create(BotWinTimesHistory.c_str(),"Arial Rounded MT bold",30,CCSizeMake(size.width/8, 0),kCCTextAlignmentRight);
    string tmpRate = int2string(MainRightRate*100) + "%";
    CCLabelTTF* _MainRate = CCLabelTTF::create(tmpRate.c_str(),"Arial Rounded MT bold",30,CCSizeMake(size.width/8, 0),kCCTextAlignmentRight);
    tmpRate = int2string(BotRightRate*100) + "%";
    CCLabelTTF* _BotRate = CCLabelTTF::create(tmpRate.c_str(),"Arial Rounded MT bold",30,CCSizeMake(size.width/8, 0),kCCTextAlignmentRight);
    CCLabelTTF* _MainCombo = CCLabelTTF::create(int2string(MainMaxCombo).c_str(),"Arial Rounded MT bold",30,CCSizeMake(size.width/8, 0),kCCTextAlignmentRight);
    CCLabelTTF* _BotCombo = CCLabelTTF::create(int2string(BotMaxCombo).c_str(),"Arial Rounded MT bold",30,CCSizeMake(size.width/8, 0),kCCTextAlignmentRight);
    _MainName->setPosition(ccp(size.width*9.3/30, size.height*54/100+winDif*alpha));
    _botName->setPosition(ccp(size.width*20.6/30, size.height*54/100+winDif*alpha));
    _MainRank->setPosition(ccp(size.width*20.5/56,size.height*49.1/100+winDif*alpha));
    _BotRank->setPosition(ccp(size.width*41.8/56, size.height*49.1/100+winDif*alpha));
    _MainWin->setPosition(ccp(size.width*20.5/56, size.height*44.4/100+winDif*alpha));
    _BotWin->setPosition(ccp(size.width*41.8/56, size.height*44.4/100+winDif*alpha));
    _MainRate->setPosition(ccp(size.width*20.5/56, size.height*40/100+winDif*alpha));
    _BotRate->setPosition(ccp(size.width*41.8/56, size.height*40/100+winDif*alpha));
    _MainCombo->setPosition(ccp(size.width*20.5/56, size.height*35.5/100+winDif*alpha));
    _BotCombo->setPosition(ccp(size.width*41.8/56, size.height*35.5/100+winDif*alpha));
    addChild(_MainName,7);
    addChild(_botName,7);
    addChild(_MainRank,7);
    addChild(_BotRank,7);
    addChild(_MainWin,7);
    addChild(_BotWin,7);
    addChild(_MainRate,7);
    addChild(_BotRate,7);
    addChild(_MainCombo,7);
    addChild(_BotCombo,7);
    
    //5.5 红绿箭头
    string mainArrowStr;
    if (MainNewRank > MainUser->Rank[tibaoType] && !isIntroducing){
        mainArrowStr = "redArrow.png";
        CCSprite* mainArrow = CCSprite::createWithSpriteFrameName(mainArrowStr.c_str());
        mainArrow->setPosition(ccp(size.width*25.3/56, size.height*48.8/100+winDif*alpha));
        addChild(mainArrow,7);
    }
    else if(MainNewRank < MainUser->Rank[tibaoType] && !isIntroducing){
        mainArrowStr = "greenArrow.png";
        CCSprite* mainArrow = CCSprite::createWithSpriteFrameName(mainArrowStr.c_str());
        mainArrow->setPosition(ccp(size.width*25.3/56, size.height*48.8/100+winDif*alpha));
        addChild(mainArrow,7);
    }//如果相等，不放箭头
    
    //5.6 记录更新后的rank
    MainUser->Rank[tibaoType] = MainNewRank;

    //5.7 写上加的金币
    CCSprite* goldSprite = CCSprite::createWithSpriteFrameName("gold_result.png");
    goldSprite->setPosition(ccp(size.width*9/20,size.height*17/70+winDif*alpha));
    addChild(goldSprite,8);
    
    string goldPlusStr = "+" + int2string(userGoldPlus);
    CCLabelTTF* goldPlusLabel = CCLabelTTF::create(goldPlusStr.c_str(),"Arial Rounded MT bold", 36);
    goldPlusLabel->setPosition(ccp(size.width*11/20, size.height*17/70+winDif*alpha));
    addChild(goldPlusLabel,8);
    
    //5.8 双方英雄
    int scaleL[6] = {1,1,-1,-1,1,1};
    int scaleR[6] = {-1,1,1,1,-1,1};
    string heroTex[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    CCSprite* LeftHero = CCSprite::createWithSpriteFrameName(heroTex[player1->rolerTag-1].c_str());
    CCSprite* RightHero = CCSprite::createWithSpriteFrameName(heroTex[bot2->rolerTag-1].c_str());
    LeftHero->setScaleX(0.65*scaleL[player1->rolerTag-1]);
    LeftHero->setScaleY(0.65);
    RightHero->setScaleX(0.65*scaleR[bot2->rolerTag-1]);
    RightHero->setScaleY(0.65);
    LeftHero->setPosition(ccp(size.width/3,size.height*4.8/7+winDif*alpha));
    RightHero->setPosition(ccp(size.width*2/3, size.height*4.8/7+winDif*alpha));
    addChild(LeftHero,5);
    addChild(RightHero,5);
    
    //5.9 皇冠
    CCPoint hgPos[2] = {ccp(size.width*12.5/50,size.height*38.5/50+winDif*alpha),ccp(size.width*38/50,size.height*38.5/50+winDif*alpha)};
    int rotation[2] = {-40,40};
    CCSprite* huangguan = CCSprite::createWithSpriteFrameName("huangguan_result.png");
    huangguan->setScale(0.7);
    huangguan->setPosition(hgPos[userSuccess]);
    huangguan->setRotation(rotation[userSuccess]);
    addChild(huangguan,6);
    
    //5.10 按钮弹出
    schedule(schedule_selector(GameBaseClass::MenuAnimation),1.5);
}


void GameBaseClass::RankMedalRecognition(){
    string medalTex;
    if (isIntroducing)
        medalTex = "medal_1.png";
    else{
        if (MainUser->Rank[tibaoType] <= 1200)
            medalTex = "medal_1.png";
        else if(MainUser->Rank[tibaoType]>1200 && MainUser->Rank[tibaoType] <= 1800)
            medalTex = "medal_2.png";
        else if(MainUser->Rank[tibaoType]>1800 && MainUser->Rank[tibaoType] <= 2500)
            medalTex = "medal_3.png";
        else if(MainUser->Rank[tibaoType]>2500 && MainUser->Rank[tibaoType] <= 3500)
            medalTex = "medal_4.png";
        else if(MainUser->Rank[tibaoType]>3500 && MainUser->Rank[tibaoType] <= 5000)
            medalTex = "medal_5.png";
        else if(MainUser->Rank[tibaoType]>5000)
            medalTex = "medal_6.png";
    }
    
    CCSprite* medal = CCSprite::createWithSpriteFrameName(medalTex.c_str());
    medal->setRotation(-30);
    medal->setScale(10);
    medal->setPosition(ccp(size.width*5.2/40, size.height*61/100+winDif));
    addChild(medal,7);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3, 0.7);
    medal->runAction(scaleDown);
}


void GameBaseClass::MenuAnimation(){
    
    unschedule(schedule_selector(GameBaseClass::MenuAnimation));
    
    RankMedalRecognition();
    
    //0.移除教学页
    removeChild(introductionLayer,true);
    
    //1.下方按钮
    CCSprite* item1Sprite = CCSprite::createWithSpriteFrameName("playagain_result.png");
    CCSprite* item1Sprite_s = CCSprite::createWithSpriteFrameName("playagain_result-s.png");
    CCSprite* item2Sprite = CCSprite::createWithSpriteFrameName("share_result.png");
    CCSprite* item2Sprite_s = CCSprite::createWithSpriteFrameName("share_result-s.png");
    CCMenuItemSprite* item1 = CCMenuItemSprite::create(item1Sprite,item1Sprite_s,this,menu_selector(GameBaseClass::ReplayGame));
    CCMenuItemSprite* item2 = CCMenuItemSprite::create(item2Sprite,item2Sprite_s,this,menu_selector(GameBaseClass::shareToSNS));
    item1->setPosition(ccp(size.width/3,-size.height/7));
    item2->setPosition(ccp(size.width*2/3, -size.height*10.2/70));
    CCMenu* resultMenu = CCMenu::create(item1,item2,NULL);
    resultMenu->setPosition(CCPointZero);
    addChild(resultMenu,6,7);
    
    //2.按钮动画
    CCActionInterval* goUp = CCMoveBy::create(1,ccp(0,size.height*21/70+winDif*alpha));
    CCActionInterval* easeElastic = CCEaseElasticInOut::create(goUp);
    resultMenu->runAction(easeElastic);
    
    //3.左上方按钮
    //CCSprite* item3Sprite = CCSprite::createWithSpriteFrameName("changeTibao_result.png");
    //CCSprite* item3Sprite_s = CCSprite::createWithSpriteFrameName("changeTibao-s_result.png");
    //CCMenuItemSprite* item3 = CCMenuItemSprite::create(item3Sprite,item3Sprite_s,this,menu_selector(GameBaseClass::backToLevelSelection));
    //item3->setPosition(ccp(size.width/10, size.height*9/10+winDif*alpha));
    //CCMenu* backMenu = CCMenu::create(item3,NULL);
    //backMenu->setPosition(CCPointZero);
    //addChild(backMenu,6,7);
}
void GameBaseClass::shareToSNS(){
    box = AsMessageBox::createMessageBox("",3,1);
    box->setPosition(CCPointZero);
    addChild(box,1000);
}
void GameBaseClass::ReplayGame(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
    
    if (isIntroducing) {
        isIntroducing = false;
        MainUser->introduced = true;
        CCUserDefault::sharedUserDefault()->setBoolForKey("Introduced", true);
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("2.Introduction.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("IronManFight.plist");
        CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
        if (MainUser->hasChosenHero)
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LevelSelectionScene::scene()));
        else
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, AsChooseHero::scene()));
    }
    
    else{
        //清理题目图片的内存
        for (int i =0 ; imageToRemove[i]!=""; i++)
            CCTextureCache::sharedTextureCache()->removeTextureForKey(imageToRemove[i].c_str());
        
        if (MainUser->hasChosenHero)
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LevelSelectionScene::scene()));
        else
            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, AsChooseHero::scene()));
        
        CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    }
}

void GameBaseClass::backToLevelSelection(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LevelSelectionScene::scene()));
}