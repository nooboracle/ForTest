#include "LogInScene.h"
#include <sys/timeb.h>
#include "global.h"
#include "ASUser.h"
#include "LevelSelectionScene.h"
#include "AsChooseHero.h"
#include "GameBaseClass.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

extern ASUser* MainUser;

void LogInScene::BasicUI(){
    //1.背景
    MainBG = CCSprite::createWithSpriteFrameName("bg_login.png");
    MainBG->setPosition(ccp(size.width/2,size.height/2+winDif*2));
    addChild(MainBG,0);
    //2.草地
    grassLand = CCSprite::createWithSpriteFrameName("GrassAndHero.png");
    grassLand->setPosition(ccp(size.width/2, size.height*480/2048+winDif*2));
    addChild(grassLand,5);
    //3.光圈
    lightCircle = CCSprite::createWithSpriteFrameName("LightCircle.png");
    lightCircle->setPosition(ccp(size.width/2, size.height*2/5+winDif*2));
    addChild(lightCircle,1);
    //4.光圈动画
    CCActionInterval* roll = CCRotateBy::create(20, 360);
    CCRepeatForever* repeat = CCRepeatForever::create(roll);
    lightCircle->runAction(repeat);
    //5.logo
    logo = CCSprite::createWithSpriteFrameName("LogoWithString.png");
    logo->setPosition(ccp(size.width/2,size.height+winDif*2.3));
    addChild(logo,3);
    //6. logo动画
    CCActionInterval* moveDown = CCMoveBy::create(1,ccp(0,-size.height/5));
    CCActionInterval* effect = CCEaseBackInOut::create(moveDown);
    logo->runAction(effect);
}

void LogInScene::ThiefKing(){
    
    CCPoint position[3] = {
        ccp(size.width/2, size.height*14.3/13+winDif*2.2),
        ccp(size.width/18, size.height/3+winDif*2.1),
        ccp(size.width*17/18,size.height/3+winDif*2.1)};
    float scale[3] = {1,0.5,0.5};
    int rotation[3] = {225,0,0};
    int zOrder[3] = {2,1,1};
    for (int i = 0 ; i < 3; i++) {
        thiefKing[i] = CCSprite::createWithSpriteFrameName("theifKing.png");
        thiefKing[i]->setScale(scale[i]);
        thiefKing[i]->setRotation(rotation[i]);
        thiefKing[i]->setPosition(position[i]);
        addChild(thiefKing[i],zOrder[i],i);
    }
    
    BeginRandomJumpOut();
}

void LogInScene::BeginRandomJumpOut(){  schedule(schedule_selector(LogInScene::randomJumpOut),7);  }

void LogInScene::randomJumpOut(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    
    struct timeb tp;
    ftime(&tp);
    std::srand(tp.millitm);
    
    CCPoint ccpBy[3] = {
        CCPoint(0,-size.height/9),
        CCPoint(0,size.height*2/27),
        CCPoint(0,size.height*2/27)};
    int tmp = rand()%3;
    CCActionInterval* jumpOut = CCMoveBy::create(0.3, ccpBy[tmp]);
    CCDelayTime* delay = CCDelayTime::create(1);
    CCActionInterval* hideBack = CCMoveBy::create(0.6, ccp(0,0 - ccpBy[tmp].y));
    CCSequence* seq = CCSequence::create(jumpOut,delay,hideBack,NULL);
    thiefKing[tmp]->runAction(seq);
}

void LogInScene::CloudUI(){
    CCSprite* cloud1 = CCSprite::createWithSpriteFrameName("cloud1.png");
    CCSprite* cloud2 = CCSprite::createWithSpriteFrameName("cloud2.png");
    cloud1->setPosition(ccp(size.width/2,size.height*2085/2048+winDif*2.4));
    cloud2->setPosition(ccp(size.width/2, size.height*1850/2048+winDif*2.4));
    addChild(cloud1,4);
    addChild(cloud2,1);
}

void LogInScene::BeginMenuFallDown(){
    
    //1.牌子的棍儿
    CCSprite* beginSprite = CCSprite::createWithSpriteFrameName("BeginButton.png");
    CCSprite* beginSprite_s = CCSprite::createWithSpriteFrameName("BeginButton-s.png");
    CCMenuItemSprite* beginItem = CCMenuItemSprite::create(beginSprite,beginSprite_s,this,menu_selector(LogInScene::goToTibaoSelection));
    beginItem->setPosition(ccp(size.width/2,size.height*6.9/6+winDif*2));
    CCMenu* BeginMenu = CCMenu::create(beginItem,NULL);
    BeginMenu->setPosition(CCPointZero);
    addChild(BeginMenu,8,997);
    
    //2.动画
    CCActionInterval* moveDown = CCMoveBy::create(0.25, ccp(0, -size.height));
    BeginMenu->runAction(moveDown);
}

void LogInScene::goToTibaoSelection(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    
    if (MainUser->introduced && MainUser->hasChosenHero)
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LevelSelectionScene::scene()));
    else if(!MainUser->introduced)
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, GameBaseClass::scene()));
    else if(MainUser->introduced && !MainUser->hasChosenHero)
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, AsChooseHero::scene()));
}

