#include "ASGame.h"
#include "AsMessageBox.h"
#include "GameBaseClass.h"
#include "ASUser.h"
#include "SimpleAudioEngine.h"
#include "global.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASGame::UIDraw(){
        
    //1.题板
    CCSprite* qBoard = CCSprite::createWithSpriteFrameName("questionBoard.png");
    qBoard->setPosition(ccp(size.width/2,size.height/2+winDif*2));
    addChild(qBoard,1);
    
    //2.手柄
    CCSprite* shoubing = CCSprite::createWithSpriteFrameName("shoubing.png");
    shoubing->setPosition(ccp(size.width/2, size.height/2+winDif*2));
    addChild(shoubing,1);
    
    //3.用户头像的框架
    CCSprite* infoFrame = CCSprite::createWithSpriteFrameName("playerInfoFrame.png");
    infoFrame->setPosition(ccp(size.width*9/40, size.height*81/90+winDif*2*alpha*alpha));
    addChild(infoFrame,3);
    
    //4.生命
    HeartBar();
    
    //5.名字
    string infoStr = "LV." + int2string(Level) + " " + MainUser->nickName;
    CCLabelTTF* userName = CCLabelTTF::create(infoStr.c_str(),"Arial Rounded MT Bold",25,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
    userName->setPosition(ccp(size.width*3.6/13,size.height*88/90+winDif*2.9*alpha));
    addChild(userName,4);
    
    //7.计时板
    CCSprite* timerTable = CCSprite::createWithSpriteFrameName("timerTable.png");
    timerTable->setPosition(ccp(size.width/2,size.height*7.3/20+winDif*2));
    addChild(timerTable,3);
    
    //6.道具栏
    CCSprite* daojulan = CCSprite::createWithSpriteFrameName("daojulan.png");
    daojulan->setPosition(ccp(size.width/2, size.height*0.8675/10));
    addChild(daojulan,1);
    
    //7.能量槽
    energyBarAnimation();
    
    //8.两个按钮
    CCSprite* button1Sprite = CCSprite::createWithSpriteFrameName("answerButton.png");
    CCSprite* button1Sprite_s = CCSprite::createWithSpriteFrameName("answerButton-s.png");
    CCSprite* button2Sprite = CCSprite::createWithSpriteFrameName("answerButton.png");
    CCSprite* button2Sprite_s = CCSprite::createWithSpriteFrameName("answerButton-s.png");
    button1 = CCMenuItemSprite::create(button1Sprite,button1Sprite_s,this,menu_selector(ASGame::userSelected));
    button2 = CCMenuItemSprite::create(button2Sprite,button2Sprite_s,this,menu_selector(ASGame::userSelected));
    button1->setTag(1*10000000+3);
    button2->setTag(1*10000000+4);
    button1->setPosition(ccp(size.width*27/96,size.height*67/256+winDif*2));
    button2->setPosition(ccp(size.width*68/96,size.height*67/256+winDif*2));
    CCMenu* menu = CCMenu::create(button1,button2,NULL);
    menu->setPosition(CCPointZero);
    addChild(menu,99,1*10000000+12);
    
    //9.蛋
    CCSprite* eggSprite = CCSprite::createWithSpriteFrameName("Egg.png");
    CCMenuItemSprite* eggItem = CCMenuItemSprite::create(eggSprite,eggSprite,this,menu_selector(ASGame::EggIsSelected));
    eggItem->setAnchorPoint(ccp(0.5, 0.5));
    eggItem->setScale(0.6);
    eggItem->setPosition(ccp(size.width*9/90, size.height*0.7375/10));
    CCMenu* eggMenu = CCMenu::create(eggItem,NULL);
    eggMenu->setPosition(CCPointZero);
    addChild(eggMenu,2);
}

void ASGame::goToNextRound(){
    
    //1.如果是教学模式,则下一步
    if (((GameBaseClass*)getParent())->isIntroducing)
        ((GameBaseClass*)getParent())->nextStep();
    else{
        unschedule(schedule_selector(ASGame::timeIsUp));
        timeCounter();
    }
    
    removeChildByTag(playerNumber*10000000+1);
    removeChildByTag(playerNumber*10000000+2);
    removeChildByTag(playerNumber*10000000+5);
    removeChildByTag(playerNumber*10000000+6);
    //removeChildByTag(playerNumber*10000000+50);
    removeChildByTag(playerNumber*10000000+51);
    removeChildByTag(playerNumber*10000000+52);
    removeChildByTag(playerNumber*10000000+53);
    removeChildByTag(playerNumber*10000000+54);
    removeChildByTag(playerNumber*10000000+13);
    
    gameRound++;
    questionDraw();
    rightOrWrongButtonDraw();
}

void ASGame::EggIsSelected(CCNode* sender){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav");
    
    CCMenuItemSprite* egg = (CCMenuItemSprite*)sender;
    CCActionInterval* rotateRight = CCRotateBy::create(0.15, 30);
    CCActionInterval* rotateBack = CCRotateBy::create(0.15, -30);
    CCActionInterval* rotateLeft = CCRotateBy::create(0.15, -30);
    CCActionInterval* rotateBack1 = CCRotateBy::create(0.15, 30);
    CCSequence* seq = CCSequence::create(rotateRight,rotateBack,rotateLeft,rotateBack1,NULL);
    egg->runAction(seq);
}
