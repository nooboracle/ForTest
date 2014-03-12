#include "RankScene.h"
#include "LevelSelectionScene.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
#include "global.h"
using namespace std;
using namespace CocosDenshion;

extern int djSelected[3];
extern int tibaoType;
extern string tibaoName[100];
extern bool djHasBought[7];
extern ASUser* MainUser;
RankScene* RankSceneLayer;
CCScene* RankScene::scene(){
    CCScene *scene = CCScene::create();
    RankSceneLayer = RankScene::create();
    scene->addChild(RankSceneLayer);
    return scene;
}

bool RankScene::init(){
    if ( !CCLayer::init() ){return false;}
    
    //1.清空上一把购买的道具信息
    for(int i = 0 ; i < 7 ; i++)
        djHasBought[i] = false;
    for (int i = 0 ; i < 3 ; i++)
        djSelected[i] = 0;
    
    //2.获取目前的体力情况
    getEnergyHTTP();
    
    //3.画其他的UI
    BasicUIDraw();
    rankListDraw(tibaoType);
    pickListDraw();
    
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    
    return true;
}

void RankScene::BasicUIDraw(){
    
    //1.背景
    CCSprite* bg = CCSprite::createWithSpriteFrameName("bg_l.png");
    bg->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(bg,0);
    
    //2.翻转层
    layerToRoll = CCLayer::create();
    addChild(layerToRoll,1);
    
    //3.金币部分
    CCSprite* heartTable = CCSprite::createWithSpriteFrameName("HeartTable.png");
    heartTable->setScaleX(0.5);
    heartTable->setScaleY(0.417);
    heartTable->setPosition(ccp(size.width*7.75/30,size.height*2100/2200+winDif*4*(2-alpha)));
    addChild(heartTable,9999);
    
    CCSprite* goldTable = CCSprite::createWithSpriteFrameName("GoldTable.png");
    goldTable->setScale(0.417);
    goldTable->setPosition(ccp(size.width*21.75/30,size.height*2100/2200+winDif*4*(2-alpha)));
    addChild(goldTable,9999);
    
    CCLabelTTF* goldNumber = CCLabelTTF::create(int2string(MainUser->gold).c_str(), "Arial Rounded MT bold",28,CCSizeMake(size.width/3, 0),kCCTextAlignmentRight);
    goldNumber->setPosition(ccp(size.width*45.5/70, size.height*2100/2200+winDif*4*(2-alpha)));
    addChild(goldNumber,10000,8);
    
    CCSprite* buyGoldButtonSprite = CCSprite::createWithSpriteFrameName("buyGoldButton.png");
    CCSprite* buyGoldButtonSprite_s = CCSprite::createWithSpriteFrameName("buyGoldButton-s.png");
    CCMenuItemSprite* buyGoldButton = CCMenuItemSprite::create(buyGoldButtonSprite,buyGoldButtonSprite_s,this,menu_selector(RankScene::showShopLayer));
    buyGoldButton->setPosition(ccp(size.width*60/70,size.height*2100/2200+winDif*4*(2-alpha)));
    CCMenu* buyGoldMenu = CCMenu::create(buyGoldButton,NULL);
    buyGoldMenu->setPosition(CCPointZero);
    addChild(buyGoldMenu,10000,17);
}

void RankScene::rankListDraw(int _tibaoType){

    tableIndex = 1;
    
    //1.主框体底板
    CCSprite* ranklistTable = CCSprite::createWithSpriteFrameName("ranklistTable.png");
    ranklistTable->setPosition(ccp(size.width/2, size.height/2+winDif*alpha*alpha));
    layerToRoll->addChild(ranklistTable,1);
    
    //2.两个模式的排行榜按钮
    CCSprite* RankListTitle = CCSprite::createWithSpriteFrameName("RankListTitle.png");
    RankListTitle->setPosition(ccp(size.width/2,size.height*1605/1700+winDif*alpha*alpha));
    layerToRoll->addChild(RankListTitle,4,9);
    
    //3.下面四个按钮
    CCSprite* backButtonSprite = CCSprite::createWithSpriteFrameName("back_rank.png");
    CCSprite* backButtonSprite_s = CCSprite::createWithSpriteFrameName("back-s_rank.png");
    CCSprite* inviteButtonSprite = CCSprite::createWithSpriteFrameName("invite_rank.png");
    CCSprite* inviteButtonSprite_s = CCSprite::createWithSpriteFrameName("invite-s_rank.png");
    CCSprite* shareButtonSprite = CCSprite::createWithSpriteFrameName("shareButton_rank.png");
    CCSprite* shareButtonSprite_s = CCSprite::createWithSpriteFrameName("shareButton-s_rank.png");
    CCSprite* ListListButtonSprite = CCSprite::createWithSpriteFrameName("ListListButton.png");
    CCSprite* ListListButtonSprite_s = CCSprite::createWithSpriteFrameName("ListListButton-s.png");
    CCMenuItemSprite* backButton = CCMenuItemSprite::create(backButtonSprite,backButtonSprite_s,this,menu_selector(RankScene::backToTibaoSelection));
    CCMenuItemSprite* inviteButton = CCMenuItemSprite::create(inviteButtonSprite,inviteButtonSprite_s,this,menu_selector(RankScene::inviteFriends));
    CCMenuItemSprite* shareButton = CCMenuItemSprite::create(shareButtonSprite,shareButtonSprite_s,this,menu_selector(RankScene::shareRank));
    CCMenuItemSprite* ListListButton = CCMenuItemSprite::create(ListListButtonSprite,ListListButtonSprite_s,this,menu_selector(RankScene::goToRankListList));
    backButton->setPosition(ccp(size.width*111/700,size.height*1017/2200+winDif*alpha*alpha));
    inviteButton->setPosition(ccp(size.width*272/700, size.height*1017/2200+winDif*alpha*alpha));
    shareButton->setPosition(ccp(size.width*430/700, size.height*1017/2200+winDif*alpha*alpha));
    ListListButton->setPosition(ccp(size.width*589/700, size.height*1017/2200+winDif*alpha*alpha));
    backButton->setTag(11);
    inviteButton->setTag(12);
    shareButton->setTag(13);
    ListListButton->setTag(14);
    bottomMenu = CCMenu::create(backButton,inviteButton,shareButton,ListListButton,NULL);
    bottomMenu->setPosition(CCPointZero);
    layerToRoll->addChild(bottomMenu,3,10);
    
    //4.上面的遮罩板
    CCSprite* rankListCover = CCSprite::createWithSpriteFrameName("rankListCover.png");
    rankListCover->setPosition(ccp(size.width*249/500,size.height*327/360+winDif*alpha*alpha));
    layerToRoll->addChild(rankListCover,3);
    
    //5.几天后结算
    daysLeftLabel = CCLabelTTF::create("0天 后结算本周排名", "Arial Rounded", 17);
    daysLeftLabel->setPosition(ccp(size.width/2, size.height*320/360+winDif*alpha*alpha));
    layerToRoll->addChild(daysLeftLabel,4,16);
    
    //6.题包名称
    CCLabelBMFont* rankName = CCLabelBMFont::create(tibaoName[_tibaoType].c_str(), "puzzlename_list2.fnt");
    rankName->setScale(0.417);
    rankName->setPosition(ccp(size.width/2, size.height*1710/1800+winDif*alpha*alpha));
    layerToRoll->addChild(rankName,5,15);
    
    //7.设置scrollview
    ContainerLayer = CCLayerColor::create(ccc4(0,0,0,0));
    ContainerLayer->setContentSize(CCSizeMake(size.width, size.height*2.14));
    ContainerLayer->setPosition(ccp(0,-size.height*1.675+winDif/130*alpha));

    //8. 1-21名的格子
    for(int i = 0 ; i < 20 ; i++){
        CCSprite* RankTable = CCSprite::createWithSpriteFrameName("other_rank.png");
        RankTable->setPosition(ccp(size.width/2, size.height*2230/1120-size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(RankTable,1,20+i);
    }
    
    //9.初始化scrollview
    CCScrollView* pScrollView = CCScrollView::create(CCSizeMake(size.width, size.height*358/768));
    pScrollView->setPosition(ccp(0, size.height/2+winDif*alpha*alpha));
    layerToRoll->addChild(pScrollView,1,999);
    pScrollView->isDragging();
    pScrollView->isTouchMoved();
    pScrollView->setBounceable(true);
    pScrollView->setContainer(ContainerLayer);
    pScrollView->getViewSize();
    pScrollView->updateInset();
    pScrollView->setTouchEnabled(true);
    pScrollView->setDelegate(this);
    pScrollView->setDirection(kCCScrollViewDirectionVertical);
}

void RankScene::disableButtons(){
    if (tableIndex == 1) {
        bottomMenu->setTouchEnabled(false);
    }
    if (tableIndex == 2) {
        chooseMenu->setTouchEnabled(false);
        bottomMenu->setTouchEnabled(false);
    }
    if (tableIndex == 3) {
        buyMenu->setTouchEnabled(false); //道具
        backMenu->setTouchEnabled(false); //返回
        //金币旁边的加号
    }
    LevelUpMenu->setTouchEnabled(false);
    moveMenu->setTouchEnabled(false);
    beginMenu->setTouchEnabled(false);
    if (LevelUpLayerHasBeenOpened)
        LevelUpMenuDetail->setTouchEnabled(false);
    CCMenu* goldMenu = (CCMenu*)this->getChildByTag(17); //购买金币
    CCMenu* bottomMenu = (CCMenu*)layerToRoll->getChildByTag(10); //下面的四个按钮
    CCMenu* leftAndRightMenu = (CCMenu*)this->getChildByTag(3);  //左右滑动
    goldMenu->setTouchEnabled(false);
    bottomMenu->setTouchEnabled(false);
    leftAndRightMenu->setTouchEnabled(false);
    
}
void RankScene::enableButtons(){
    if (tableIndex == 1) {
        bottomMenu->setTouchEnabled(true);
    }
    if (tableIndex == 2) {
        chooseMenu->setTouchEnabled(true);
        bottomMenu->setTouchEnabled(true);
    }
    if (tableIndex == 3) {
        buyMenu->setTouchEnabled(true); //道具
        backMenu->setTouchEnabled(true); //返回
        //金币旁边的加号
    }
    LevelUpMenu->setTouchEnabled(true);
    moveMenu->setTouchEnabled(true);
    beginMenu->setTouchEnabled(true);
    if (LevelUpLayerHasBeenOpened)
        LevelUpMenuDetail->setTouchEnabled(true);
    CCMenu* goldMenu = (CCMenu*)this->getChildByTag(17); //购买金币
    CCMenu* bottomMenu = (CCMenu*)layerToRoll->getChildByTag(10); //下面的四个按钮
    CCMenu* leftAndRightMenu = (CCMenu*)this->getChildByTag(3);  //左右滑动
    goldMenu->setTouchEnabled(true);
    bottomMenu->setTouchEnabled(true);
    leftAndRightMenu->setTouchEnabled(true);
}

void RankScene::backToTibaoSelection(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LevelSelectionScene::scene()));
}
void RankScene::shareRank(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    
    MessageBox = AsMessageBox::createMessageBox("即将开放\n敬请期待", 2, 3);
    MessageBox->setPosition(CCPointZero);
    addChild(MessageBox,10001);
}
void RankScene::inviteFriends(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    
    MessageBox = AsMessageBox::createMessageBox("", 3, 0);
    MessageBox->setPosition(CCPointZero);
    addChild(MessageBox,10001);
}
void RankScene::scrollViewDidScroll(CCScrollView* view){}
void RankScene::scrollViewDidZoom(CCScrollView* view){}