#include "AsMessageBox.h"
#include "ASUser.h"
#include "IOSBridge.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

extern ASUser* MainUser;
extern int shareImageIndex;

void AsMessageBox::initShareMessageBox(int subType){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("openFrame.wav");
    
    shareImageIndex = subType;
    
    //1.截屏
    if (subType == 1) {
        CCRenderTexture* pScreen = CCRenderTexture::create(winSize.width,winSize.height);
        CCScene* pCurScene = CCDirector::sharedDirector()->getRunningScene();
        pScreen->begin();
        pCurScene->visit();
        pScreen->end();
        pScreen->saveToFile("shared.png",kCCImageFormatPNG);
    }

    //2.黑色底层
    cover = CCLayerColor::create(ccc4(0,0,0,200));
    cover->setPosition(CCPointZero);
    addChild(cover,0);
    
    //3.提示框底层
    showbox = CCSprite::createWithSpriteFrameName("shareFrame.png");
    showbox->setScaleY(1.1);
    showbox->setPosition(ccp(size.width/2,size.height/2+winDif*2*alpha));
    cover->addChild(showbox,1);
    
    //4.提示框上的文字
    CCSprite* shareWeixinSprite = CCSprite::createWithSpriteFrameName("shareWeixin.png");
    CCSprite* shareWeixinSprite_s = CCSprite::createWithSpriteFrameName("shareWeixin-s.png");
    CCSprite* shareWeixin1Sprite = CCSprite::createWithSpriteFrameName("shareWeixin1.png");
    CCSprite* shareWeixin1Sprite_s = CCSprite::createWithSpriteFrameName("shareWeixin1-s.png");
    CCSprite* closeSprite = CCSprite::createWithSpriteFrameName("closeMatchTable.png");
    CCSprite* closeSprite_s = CCSprite::createWithSpriteFrameName("closeMatchTable-s.png");
    CCMenuItemSprite* closeItem = CCMenuItemSprite::create(closeSprite,closeSprite_s,this,menu_selector(AsMessageBox::closeMessageBox));
    CCMenuItemSprite* item1 = CCMenuItemSprite::create(shareWeixinSprite,shareWeixinSprite_s,this,menu_selector(AsMessageBox::goShare));
    CCMenuItemSprite* item2 = CCMenuItemSprite::create(shareWeixin1Sprite,shareWeixin1Sprite_s,this,menu_selector(AsMessageBox::goShare));
    item1->setTag(10);
    item2->setTag(11);
    item1->setPosition(ccp(size.width*18/50,size.height*10/20+winDif*2.18*alpha));
    item2->setPosition(ccp(size.width*32/50,size.height*10/20+winDif*2.18*alpha));
    closeItem->setPosition(ccp(size.width*5.7/7,size.height*11.5/20+winDif*2.18*alpha));
    closeMenu = CCMenu::create(item1,item2,closeItem,NULL);
    closeMenu->setPosition(CCPointZero);
    addChild(closeMenu,2);
    
    //5.文字提示
    CCLabelTTF* shareToIndiLabel = CCLabelTTF::create("分享给朋友","Arial Rounded MT Bold",24);
    CCLabelTTF* shareToCircleLabel = CCLabelTTF::create("分享到朋友圈","Arial Rounded MT Bold",24);
    shareToIndiLabel->setColor(ccc3(55, 55, 55));
    shareToCircleLabel->setColor(ccc3(55, 55, 55));
    shareToIndiLabel->setPosition(ccp(size.width*18/50,size.height*8/20+winDif*2.18*alpha));
    shareToCircleLabel->setPosition(ccp(size.width*32/50,size.height*8/20+winDif*2.18*alpha));
    addChild(shareToIndiLabel,7,53);
    addChild(shareToCircleLabel,7,54);
    
}

void AsMessageBox::goShare(CCNode* sender){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.mp3");
    int tmp = ((CCMenuItemSprite*)sender)->getTag() - 10;
    if(!IOSBridge::WXImageIOS(tmp)){
        closeMessageBox();
        CCMessageBox("您的设备中好像木有安装微信", "温馨提示");
    }
}