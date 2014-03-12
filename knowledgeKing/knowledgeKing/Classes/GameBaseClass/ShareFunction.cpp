#include "SimpleAudioEngine.h"
#include "IOSBridge.h"
#include<stdio.h>
#include "GameBaseClass.h"
using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

//extern int gold;
bool sharing;

void GameBaseClass::shareToSNS(){
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    
    //1.screen cut
    CCRenderTexture* pScreen = CCRenderTexture::create(640,960);
    CCScene* pCurScene = CCDirector::sharedDirector()->getRunningScene();
    pScreen->begin();
    pCurScene->visit();
    pScreen->end();
    pScreen->saveToFile("shared.png",kCCImageFormatPNG);
    
    //2.draw cover
    CCLayerColor* cover = CCLayerColor::create(ccc4(0,0,0,150));
    addChild(cover,5,50);
    
    CCSprite* shareCover = CCSprite::create("shareCover.png");
    shareCover->setPosition(ccp(320,-80));
    addChild(shareCover,5,51);
    
    //3.draw share buttons
    CCMenuItemImage* shareToIndi = CCMenuItemImage::create("weixinLogo.png","weixinLogo-s.png",this,menu_selector(GameBaseClass::shareToFriend));
    CCMenuItemImage* shareToCircle = CCMenuItemImage::create("weixinLogo.png","weixinLogo-s.png",this,menu_selector(GameBaseClass::shareToFriendCircle));
    shareToIndi->setPosition(ccp(160,-130));
    shareToCircle->setPosition(ccp(320,-130));
    CCMenu* shareMenu = CCMenu::create(shareToIndi,shareToCircle,NULL);
    shareMenu->setPosition(CCPointZero);
    addChild(shareMenu,6,52);
    
    CCLabelTTF* shareToIndiLabel = CCLabelTTF::create("分享给朋友","Arial",24);
    CCLabelTTF* shareToCircleLabel = CCLabelTTF::create("分享到朋友圈","Arial",24);
    shareToIndiLabel->setPosition(ccp(160,-200));
    shareToCircleLabel->setPosition(ccp(320,-200));
    addChild(shareToIndiLabel,7,53);
    addChild(shareToCircleLabel,7,54);
    
    //4.action
    CCActionInterval* goUp = CCMoveBy::create(0.2,ccp(0,320));
    shareCover->runAction(goUp);
    CCActionInterval* goUp_Menu = CCMoveBy::create(0.2,ccp(0,320));
    shareMenu->runAction(goUp_Menu);
    CCActionInterval* goUp_Label_1 = CCMoveBy::create(0.2,ccp(0,320));
    shareToIndiLabel->runAction(goUp_Label_1);
    CCActionInterval* goUp_Label_2 = CCMoveBy::create(0.2,ccp(0,320));
    shareToCircleLabel->runAction(goUp_Label_2);
    
    //5.unable buttons
    congmenu->setTouchEnabled(false);
    
    sharing = true;
}

void GameBaseClass::closeShareCover(){
    
    sharing = false;
    congmenu->setTouchEnabled(true);
    
    removeChildByTag(50);
    CCSprite* shareCover = (CCSprite*)this->getChildByTag(51);
    CCMenu* shareMenu = (CCMenu*)this->getChildByTag(52);
    CCLabelTTF* labe_1 = (CCLabelTTF*)this->getChildByTag(53);
    CCLabelTTF* labe_2 = (CCLabelTTF*)this->getChildByTag(54);
    CCActionInterval* goDown = CCMoveBy::create(0.2,ccp(0,-320));
    CCActionInterval* goDown_1 = CCMoveBy::create(0.2,ccp(0,-320));
    CCActionInterval* goDown_2 = CCMoveBy::create(0.2,ccp(0,-320));
    CCActionInterval* goDown_3 = CCMoveBy::create(0.2,ccp(0,-320));
    
    CCCallFunc* removeShare = CCCallFuncN::create(this,callfuncN_selector(GameBaseClass::removeShareElements));
    CCSequence* sequence = CCSequence::create(goDown, removeShare, NULL);
    CCSequence* sequence_1 = CCSequence::create(goDown_1, removeShare, NULL);
    CCSequence* sequence_2 = CCSequence::create(goDown_2, removeShare, NULL);
    CCSequence* sequence_3 = CCSequence::create(goDown_3, removeShare, NULL);
    
    shareCover->runAction(sequence);
    shareMenu->runAction(sequence_1);
    labe_1->runAction(sequence_2);
    labe_2->runAction(sequence_3);
}


void GameBaseClass::shareToFriend(){
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    IOSBridge::WXImageIOS(0);
    
    //gold += 30;
    //CCUserDefault::sharedUserDefault()->setIntegerForKey("GoldData",gold);
}

void GameBaseClass::shareToFriendCircle(){
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    IOSBridge::WXImageIOS(1);
    
    //gold += 30;
    //CCUserDefault::sharedUserDefault()->setIntegerForKey("GoldData",gold);
}

void GameBaseClass::removeShareElements(CCNode* sender){removeChild(sender);}

