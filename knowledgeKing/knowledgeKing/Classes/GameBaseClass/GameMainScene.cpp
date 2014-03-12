#include "GameBaseClass.h"
#include "ASGame.h"
#include "ASUser.h"
#include "ASBot.h"
#include "SimpleAudioEngine.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;
extern ASGame* player1;
extern ASBot* bot2;
extern int djMaxUseNumber[7];

CCScene* GameBaseClass::scene(){
    CCScene *scene = CCScene::create();
    GameBaseClass* GameMainLayer = GameBaseClass::create();
    scene->addChild(GameMainLayer);
    return scene;
}

bool GameBaseClass::init(){
    if ( !CCLayer::init() ){return false;}
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("FightGeneral.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ItemEffect.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("LichKingFight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("IronManFight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SheldonFight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("BladeMasterFight.plist");
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.05);
    
    //1.正式游戏
    if (MainUser->introduced)
        beginOnlineGame();
    
    //2.教学
    else{
        beginGameIntroduction();
    }
    
    return true;
}

void GameBaseClass::beginGameIntroduction(){

    isIntroducing = true;
    screenCanBeTouched = true;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("IronManFight.plist");

    
    //0. 教学层
    introductionLayer = CCLayer::create();
    introductionLayer->setPosition(CCPointZero);
    addChild(introductionLayer,4);
    
    //1.背景
    CCSprite* underBG = CCSprite::createWithSpriteFrameName("bg_l.png");
    underBG->setPosition(ccp(size.width/2,size.height/2+winDif*2*alpha*alpha));
    addChild(underBG,0);
    
    CCSprite* upperbg = CCSprite::createWithSpriteFrameName("bg_empty_l.png");
    upperbg->setPosition(ccp(size.width/2,size.height/2+winDif*2*alpha*alpha));
    addChild(upperbg,2,999);
    
    //2.主玩家
    player1 =  ASGame::createGame(2,1,true);
    player1->setPosition(CCPointZero);
    addChild(player1,3,99999999);
    
    //2.1 主玩家头像
    string nameStr[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    CCSprite* heroImage = CCSprite::createWithSpriteFrameName(nameStr[player1->rolerTag-1].c_str());
    heroImage->setScale(0.5);
    heroImage->setPosition(ccp(size.width*5/40,size.height*80/90+winDif*2*alpha*alpha));
    addChild(heroImage,1,1*10000000+60);
    
    //2.2 初始化主玩家道具
    for (int i = 0 ; i < 3 ; i++)
        player1->dj_tag[i] = -1;
    for (int i = 0 ; i < 3 ; i++) {
        player1->dj_tag[i] = i+1;
        player1->dj_number[i] = djMaxUseNumber[i];
    }
    
    //3.机器人
    int tmp[50];
    bot2 =  ASBot::createBot(2,2,0,0,0,0,"天天被欺负","0","a",true,666,120,0,tmp);
    bot2->setPosition(ccp(0,size.height*8.21/10));
    addChild(bot2,3,99999998);
    
    //3.1 机器人头像
    CCSprite* botheroImage = CCSprite::createWithSpriteFrameName(nameStr[bot2->rolerTag-1].c_str());
    botheroImage->setPosition(ccp(size.width*35/40,size.height*80/90+winDif*2*alpha*alpha));
    botheroImage->setScale(0.5);
    addChild(botheroImage,1,2*10000000+60);
    
    //4.开始教学
    player1->UIDraw();
    player1->ASdjUI();
    player1->questionDraw();
    player1->rightOrWrongButtonDraw();
    bot2->UIDraw();
    IntroStep1();
}





