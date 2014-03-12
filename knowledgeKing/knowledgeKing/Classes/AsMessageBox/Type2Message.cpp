#include "ASMessageBox.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
#include "RankScene.h"
using namespace CocosDenshion;
extern ASUser* MainUser;
extern RankScene* RankSceneLayer;

void AsMessageBox::initMessageBox(string _message,int _subType){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav");
    
    //1.黑色底层
    cover = CCLayerColor::create(ccc4(0,0,0,200));
    cover->setPosition(CCPointZero);
    addChild(cover,0);
    
    //2.提示框底层
    showbox = CCSprite::createWithSpriteFrameName("MessageFrame.png");
    showbox->setPosition(ccp(size.width/2,size.height/2+winDif*2*alpha));
    cover->addChild(showbox,1);
    
    //3.提示框上的文字
    message = CCLabelTTF::create(_message.c_str(), "Arial Rounded MT bold", 36, CCSizeMake(size.width/2, 0),kCCTextAlignmentCenter);
    message->setPosition(ccp(size.width/2,size.height*11/20+winDif*2*alpha));
    cover->addChild(message,2);
    
    if (_subType == 1)
        addConfirmButton();
    else if(_subType == 2)
        addGoToShopButton();
    else if(_subType == 3){
        CCDelayTime* delay = CCDelayTime::create(1);
        CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(AsMessageBox::closeMessageBox));
        CCSequence* seq = CCSequence::create(delay,remove,NULL);
        runAction(seq);
    }
    else if(_subType == 4)
        addBuyEnergyButton();
}

void AsMessageBox::addConfirmButton(){
    
    CCSprite* closeSprite = CCSprite::createWithSpriteFrameName("ConfirmButton.png");
    CCSprite* closeSprite_s = CCSprite::createWithSpriteFrameName("ConfirmButton-s.png");
    CCMenuItemSprite* closeShowBox = CCMenuItemSprite::create(closeSprite,closeSprite_s,this,menu_selector(AsMessageBox::closeMessageBox));
    closeShowBox->setPosition(ccp(size.width/2,size.height*36/90+winDif*2*alpha));
    closeMenu = CCMenu::create(closeShowBox,NULL);
    closeMenu->setPosition(CCPointZero);
    cover->addChild(closeMenu,12);
}

void AsMessageBox::addGoToShopButton(){
    CCSprite* goToShopSprite = CCSprite::createWithSpriteFrameName("buyItemButton.png");
    CCSprite* goToShopSprite_s = CCSprite::createWithSpriteFrameName("buyItemButton-s.png");
    CCSprite* closeShowBoxSprite = CCSprite::createWithSpriteFrameName("unbuyItemButton.png");
    CCSprite* closeShowBoxSprite_s = CCSprite::createWithSpriteFrameName("unbuyItemButton-s.png");
    CCMenuItemSprite* goToShop = CCMenuItemSprite::create(goToShopSprite,goToShopSprite_s,RankSceneLayer,menu_selector(RankScene::showShopLayer));
    CCMenuItemSprite* closeShowBox = CCMenuItemSprite::create(closeShowBoxSprite,closeShowBoxSprite_s,this,menu_selector(AsMessageBox::closeMessageBox));
    goToShop->setPosition(ccp(size.width*10.5/30,size.height*36/90+winDif*2*alpha));
    closeShowBox->setPosition(ccp(size.width*19.5/30,size.height*36/90+winDif*2*alpha));
    closeMenu = CCMenu::create(goToShop,closeShowBox,NULL);
    closeMenu->setPosition(CCPointZero);
    cover->addChild(closeMenu,12);
}

void AsMessageBox::addBuyEnergyButton(){
    
    CCSprite* goToShopSprite = CCSprite::createWithSpriteFrameName("buyItemButton.png");
    CCSprite* goToShopSprite_s = CCSprite::createWithSpriteFrameName("buyItemButton-s.png");
    CCSprite* closeShowBoxSprite = CCSprite::createWithSpriteFrameName("unbuyItemButton.png");
    CCSprite* closeShowBoxSprite_s = CCSprite::createWithSpriteFrameName("unbuyItemButton-s.png");
    CCMenuItemSprite* goToShop = CCMenuItemSprite::create(goToShopSprite,goToShopSprite_s,this,menu_selector(AsMessageBox::buyEnergy));
    CCMenuItemSprite* closeShowBox = CCMenuItemSprite::create(closeShowBoxSprite,closeShowBoxSprite_s,this,menu_selector(AsMessageBox::closeMessageBox));
    goToShop->setPosition(ccp(size.width*10.5/30,size.height*36/90+winDif*2*alpha));
    closeShowBox->setPosition(ccp(size.width*19.5/30,size.height*36/90+winDif*2*alpha));
    closeMenu = CCMenu::create(goToShop,closeShowBox,NULL);
    closeMenu->setPosition(CCPointZero);
    cover->addChild(closeMenu,12);
}

void AsMessageBox::buyEnergy(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav");
    
    RankSceneLayer->buyEnergy();
}