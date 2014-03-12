#include "LogInScene.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace CocosDenshion;
using namespace std;

extern ASUser* MainUser;

CCScene* LogInScene::scene(){
    CCScene *scene = CCScene::create();
    LogInScene *layer = LogInScene::create();
    scene->addChild(layer);
    return scene;
}

bool LogInScene::init(){
    if ( !CCLayer::init() ){return false;}
    
    BasicUI();
    popUpMenuDraw();
    CloudUI();
    ThiefKing();
    
    if (!MainUser->hasLogIn)
        LogInTableDraw(2);
    else
        BeginMenuFallDown();
    
    if(!MainUser->muted){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("123.mp3", true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.25);
    }
    
    return true;
}

void LogInScene::popUpMenuDraw(){
    
    //1.基础层
    popUpMenuLayer = CCLayer::create();
    popUpMenuLayer->setPosition(CCPointZero);
    addChild(popUpMenuLayer,9);
    
    //2.弹出条
    CCSprite* popUpTable = CCSprite::createWithSpriteFrameName("popUpTable.png");
    popUpTable->setPosition(ccp(-size.width*25/110,size.height/4+winDif*2));
    popUpMenuLayer->addChild(popUpTable,0);
    
    //3.弹出条按钮
    CCSprite* popUpSprite = CCSprite::createWithSpriteFrameName("popUpButton.png");
    CCSprite* popUpSprite_s = CCSprite::createWithSpriteFrameName("popUpButton-s.png");
    CCMenuItemSprite* popUpItem = CCMenuItemSprite::create(popUpSprite, popUpSprite_s,this,menu_selector(LogInScene::popUpMenuMove));
    popUpItem->setPosition(ccp(size.width*6/110,size.height/4+winDif*2));
    CCMenu* popUpMenu = CCMenu::create(popUpItem,NULL);
    popUpMenu->setPosition(CCPointZero);
    popUpMenuLayer->addChild(popUpMenu,1,998);
    
    //4.两个按钮
    CCSprite* accountSprite = CCSprite::createWithSpriteFrameName("AccountManagementButton.png");
    CCSprite* accountSprite_s = CCSprite::createWithSpriteFrameName("AccountManagementButton-s.png");
    CCSprite* teachSprite = CCSprite::createWithSpriteFrameName("settingsButton.png");
    CCSprite* teachSprite_s = CCSprite::createWithSpriteFrameName("settingsButton-s.png");
    CCMenuItemSprite* accountItem = CCMenuItemSprite::create(accountSprite,accountSprite_s,this,menu_selector(LogInScene::openAccountManagementLayer));
    CCMenuItemSprite* teachItem = CCMenuItemSprite::create(teachSprite,teachSprite_s,this,menu_selector(LogInScene::goToIntroduction));
    accountItem->setPosition(ccp(-size.width*14.25/110, size.height/4+winDif*2));
    teachItem->setPosition(ccp(-size.width*20/100, size.height/4+winDif*2));
    CCMenu* menu = CCMenu::create(accountItem,NULL);
    menu->setPosition(CCPointZero);
    popUpMenuLayer->addChild(menu,2);
}

void LogInScene::popUpMenuMove(){
    if (popUpMenuCounter == 1) {
        
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav",false);
        
        CCActionInterval* move = CCMoveBy::create(0.3, ccp(size.width*3/11,0));
        popUpMenuLayer->runAction(move);
        popUpMenuCounter = -1;
        
    }else{
        
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("closeFrame.wav",false);
        
        CCActionInterval* move = CCMoveBy::create(0.3, ccp(-size.width*3/11,0));
        popUpMenuLayer->runAction(move);
        popUpMenuCounter = 1;
    }
}

void LogInScene::openAccountManagementLayer(){
    
    CCMenu* popUpMenu = (CCMenu*)popUpMenuLayer->getChildByTag(998);
    CCMenu* beginMenu = (CCMenu*)this->getChildByTag(997);
    popUpMenu->setTouchEnabled(false);
    if (beginMenu!=NULL)
        beginMenu->setTouchEnabled(false);
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav",false);
        
    popUpMenuMove();
    
    CCLayerColor* black = CCLayerColor::create(ccc4(0, 0, 0, 155));
    black->setPosition(CCPointZero);
    addChild(black,10);
    
    CCSprite* table = CCSprite::createWithSpriteFrameName("AccountFrame.png");
    table->setPosition(ccp(size.width/2,size.height/2+winDif*2.5));
    black->addChild(table,0);
    
    string nickStr = "当前账户:" + MainUser->nickName;
    CCLabelTTF* nick = CCLabelTTF::create(nickStr.c_str(), "Arial Rounded MT bold", 30);
    nick->setPosition(ccp(size.width/2, size.height*45/70+winDif*2.5));
    nick->setColor(ccc3(50, 50, 50));
    black->addChild(nick,1);
    
    CCSprite* item1Sprite = CCSprite::createWithSpriteFrameName("signUpButton.png");
    CCSprite* item1Sprite_s = CCSprite::createWithSpriteFrameName("signUpButton-s.png");
    CCSprite* item2Sprite = CCSprite::createWithSpriteFrameName("changeAccount.png");
    CCSprite* item2Sprite_s = CCSprite::createWithSpriteFrameName("changeAccount-s.png");
    CCSprite* item3Sprite = CCSprite::createWithSpriteFrameName("bindAccount.png");
    CCSprite* item3Sprite_s = CCSprite::createWithSpriteFrameName("bindAccount-s.png");
    CCSprite* item4Sprite = CCSprite::createWithSpriteFrameName("closeMatchTable.png");
    CCSprite* item4Sprite_s = CCSprite::createWithSpriteFrameName("closeMatchTable-s.png");
    CCMenuItemSprite* item1 = CCMenuItemSprite::create(item1Sprite,item1Sprite_s,this,menu_selector(LogInScene::openSignUpTable));
    CCMenuItemSprite* item2 = CCMenuItemSprite::create(item2Sprite,item2Sprite_s,this,menu_selector(LogInScene::openChangeAccountTable));
    CCMenuItemSprite* item3 = CCMenuItemSprite::create(item3Sprite,item3Sprite_s,this,menu_selector(LogInScene::openBindingTable));
    CCMenuItemSprite* item4 = CCMenuItemSprite::create(item4Sprite,item4Sprite_s,this,menu_selector(LogInScene::closeThisLayer));
    item1->setPosition(ccp(size.width/2, size.height*40/70+winDif*2.5));
    item2->setPosition(ccp(size.width/2, size.height*26/70+winDif*2.5));
    item3->setPosition(ccp(size.width/2, size.height*33/70+winDif*2.5));
    item4->setPosition(ccp(size.width*22/30, size.height*20.5/30+winDif*2.5));
    CCMenu* menu = CCMenu::create(item1,item2,item3,item4,NULL);
    menu->setPosition(CCPointZero);
    black->addChild(menu,1);
}

void LogInScene::openSignUpTable(CCNode* sender){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav",false);
    sender->getParent()->getParent()->getParent()->removeChild(sender->getParent()->getParent());
    LogInTableDraw(0);
}
void LogInScene::openBindingTable(CCNode* sender){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav",false);
    sender->getParent()->getParent()->getParent()->removeChild(sender->getParent()->getParent());
    LogInTableDraw(1);
}
void LogInScene::openChangeAccountTable(CCNode* sender){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav",false);
    sender->getParent()->getParent()->getParent()->removeChild(sender->getParent()->getParent());
    LogInTableDraw(2);
}
void LogInScene::closeThisLayer(CCNode* sender){
    
    CCMenu* popUpMenu = (CCMenu*)popUpMenuLayer->getChildByTag(998);
    CCMenu* beginMenu = (CCMenu*)this->getChildByTag(997);
    popUpMenu->setTouchEnabled(true);
    if (beginMenu!=NULL)
        beginMenu->setTouchEnabled(true);

    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("closeFrame.wav",false);
    sender->getParent()->getParent()->getParent()->removeChild(sender->getParent()->getParent(),true);
}

void LogInScene::goToIntroduction(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav",false);
    box = AsMessageBox::createMessageBox("即将进入教学模式", 2, 3);
    box->setPosition(CCPointZero);
    addChild(box,100);
}