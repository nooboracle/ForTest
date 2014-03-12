#include "RankScene.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;
extern int tibaoType;
extern string djDesc[7];
extern int djPrice[7];
extern string tibaoName[100];
extern bool djHasBought[7];

void RankScene::djToRankList(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    
    tableIndex = 1;
    CCCallFunc* stableDjList = CCCallFuncN::create(this, callfuncN_selector(RankScene::stableDaoJuList));
    CCActionInterval* firstHalf = CCOrbitCamera::create(0.35, 0.5, 0, 0, 90, 0, 0);
    CCCallFunc* middle = CCCallFuncN::create(this, callfuncN_selector(RankScene::middleProcessFromDjToRankList));
    CCActionInterval* secondHalf = CCOrbitCamera::create(0.35, 0.5, 0, 270, 90, 0, 0);
    CCCallFunc* resetContent = CCCallFuncN::create(this, callfuncN_selector(RankScene::resetToRankList));
    CCSequence* seq = CCSequence::create(stableDjList,firstHalf,middle,secondHalf,resetContent,NULL);
    layerToRoll->runAction(seq);
}

void RankScene::stableDaoJuList(){
    
    //1.获得当前scrollview的位置，这样翻转时候就不会出现列表错位
    
    //2.去掉scrollview，这样翻转时候就不会有一条横线了
    layerToRoll->removeChildByTag(999);
    
    //3.把原来scrollview上的东西再放回去
    //3.1 containerlayer
    ContainerLayer = CCLayerColor::create(ccc4(0,0,0,0));
    ContainerLayer->setPosition(ccp(0, 0+winDif*alpha*alpha));
    layerToRoll->addChild(ContainerLayer,2,998);
    
    //3.2 前20名的板子
    for(int i = 0 ; i < 4 ; i++){
        CCSprite* RankTable = CCSprite::createWithSpriteFrameName("other_rank.png");
        RankTable->setPosition(ccp(size.width/2, size.height*915/1120-size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(RankTable,1,20+i);
    }
    
    buyMenu = MyMenu::create();
    for (int i = 0 ; i < 4 ; i++) {
        //1.道具名称
        string itemTexName = "daoju_" + int2string(i+1) + ".png";
        CCSprite* itemImage = CCSprite::createWithSpriteFrameName(itemTexName.c_str());
        itemImage->setScale(0.8);
        itemImage->setPosition(ccp(size.width*180/1120, size.height*918/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(itemImage,4,160+i);
        
        //2.价格
        CCSprite* priceTable = CCSprite::createWithSpriteFrameName("priceTable.png");
        priceTable->setPosition(ccp(size.width*420/1120,size.height*935/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(priceTable,3,260+i);
        
        CCLabelTTF* itemPrice = CCLabelTTF::create(int2string(djPrice[i]).c_str(),"Arial Rounded MT bold",28,CCPoint(size.width,0),kCCTextAlignmentLeft);
        itemPrice->enableShadow(CCSizeMake(size.width/600, -size.height/500), 255, 0.5);
        itemPrice->setPosition(ccp(size.width*920/1120, size.height*935/1120 - size.height*i*63/660+winDif/130*alpha));
        itemPrice->setColor(ccc3(245,245,245));
        ContainerLayer->addChild(itemPrice,4,360+i);
        
        //3.描述
        CCLabelTTF* desc = CCLabelTTF::create(djDesc[i].c_str(), "Arial Rounded MT bold", 25 , CCPoint(size.width,0),kCCTextAlignmentLeft);
        desc->setPosition(ccp(size.width*840/1120, size.height*895/1120 - size.height*i*63/660+winDif/130*alpha));
        desc->setColor(ccc3(30,102,129));
        ContainerLayer->addChild(desc,4,460+i);
        
        //4.购买提示
        if (djHasBought[i]) {
            CCSprite* boughtTag = CCSprite::createWithSpriteFrameName("greenMark.png");
            boughtTag->setPosition(ccp(size.width*130/1120, size.height*943/1120 - size.height*i*63/660+winDif/130*alpha));
            ContainerLayer->addChild(boughtTag,5,560+i);
        }
        
        //5.购买按钮
        CCSprite* buyButtonSprite = CCSprite::createWithSpriteFrameName("buyItemButton.png");
        CCSprite* buyButtonSprite_s = CCSprite::createWithSpriteFrameName("buyItemButton-s.png");
        CCMenuItemSprite* buyItem = CCMenuItemSprite::create(buyButtonSprite,buyButtonSprite_s,this, menu_selector(RankScene::chooseDj));
        buyItem->setScale(0.8);
        buyItem->setPosition(ccp(size.width*930/1120, size.height*915/1120 - size.height*i*63/660+winDif/130*alpha));
        buyItem->setTag(1001+i);
        buyMenu->addChild(buyItem);
        buyMenu->setPosition(CCPointZero);
        
    }
    ContainerLayer->addChild(buyMenu,400,660);
}

void RankScene::middleProcessFromDjToRankList(){
   
    //1.去掉上一个表的内容
    layerToRoll->removeChildByTag(7);
    layerToRoll->removeChildByTag(8);
    layerToRoll->removeChildByTag(9);
    layerToRoll->removeChildByTag(10);
    layerToRoll->removeChildByTag(11);
    layerToRoll->removeChildByTag(17);
    layerToRoll->removeChildByTag(40);
    CCLabelBMFont* rankName = (CCLabelBMFont*)layerToRoll->getChildByTag(15);
    rankName->setString(tibaoName[tibaoType].c_str());
    for (int i = 0 ; i < 20 ; i++) {
        ContainerLayer->removeChildByTag(160 + i);
        ContainerLayer->removeChildByTag(260 + i);
        ContainerLayer->removeChildByTag(360 + i);
        ContainerLayer->removeChildByTag(460 + i);
        ContainerLayer->removeChildByTag(560 + i);
        ContainerLayer->removeChildByTag(660 + i);
    }
    
    //3.写上这一个表的内容
    //3.1 排行榜信息
    for (int i = 0 ; i < 4 ; i++) {
        //3.1 图片
        CCSprite* userImageItem = CCSprite::createWithSpriteFrameName(userImage[i].c_str());
        userImageItem->setScale(0.5);
        userImageItem->setPosition(ccp(size.width*294/1120, size.height*915/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(userImageItem,4,160+i);
        
        //3.2 用户名
        if (nickName_Rank[i] == "")
            nickName_Rank[i] = "暂无人上榜";
        CCLabelTTF* name = CCLabelTTF::create(nickName_Rank[i].c_str(), "Arial Rounded MT Bold", 23,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
        name->setColor(ccc3(50, 50, 50));
        name->setPosition(ccp(size.width*655/1120, size.height*935/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(name,4,260+i);
        
        //3.3 胜率
        CCLabelTTF* winPercentLabel = CCLabelTTF::create("胜率", "Arial Rounded MT Bold",32,CCSizeMake(size.width/5, 0),kCCTextAlignmentLeft);
        winPercentLabel->setColor(ccc3(149, 70, 15));
        winPercentLabel->setPosition(ccp(size.width*815/1120,size.height*915/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(winPercentLabel,4,760+i);
        
        string weeklyWinRateStr = winPercent_Rank[i];
        if (winPercent_Rank[i] == "")
            weeklyWinRateStr = "0%";
        CCLabelTTF* winPercent = CCLabelTTF::create(weeklyWinRateStr.c_str(), "Thonburi-Bold",32,CCSizeMake(size.width/3, 0),kCCTextAlignmentRight);
        winPercent->setColor(ccc3(28,104,117));
        winPercent->setPosition(ccp(size.width*820/1120, size.height*916/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(winPercent,4,360+i);
        
        //3.4 胜场
        CCLabelTTF* winTimesLabel = CCLabelTTF::create("胜场", "Arial Rounded MT Bold", 23,CCSizeMake(size.width/10, 0),kCCTextAlignmentLeft);
        winTimesLabel->setColor(ccc3(149, 70, 15));
        winTimesLabel->setPosition(ccp(size.width*434/1120,size.height*895/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(winTimesLabel,4,660+i);
        
        string weeklyWinStr;
        if (weeklyWin_Rank[i]!="")
            weeklyWinStr = weeklyWin_Rank[i];
        else
            weeklyWinStr = "0";
        CCLabelTTF* weeklywin = CCLabelTTF::create(weeklyWinStr.c_str(), "Verdana-Bold",24,CCSizeMake(size.width/5, 0),kCCTextAlignmentLeft);
        weeklywin->setColor(ccc3(28,104,117));
        weeklywin->setPosition(ccp(size.width*580/1120, size.height*894.5/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(weeklywin,4,460+i);
        
        //3.5 名次数字
        if (i > 2) {
            CCLabelBMFont* classnumber = CCLabelBMFont::create(int2string(i+1).c_str(), "classnum.fnt");
            classnumber->setScale(0.417);
            classnumber->setPosition(ccp(size.width*146/1120, size.height*915/1120 - size.height*i*63/660+winDif/130*alpha));
            ContainerLayer->addChild(classnumber,4,560+i);
        }
    }
    
    string hg[3] = {"goldHG.png","silverHG.png","bronzeHG.png"};
    for (int i = 0 ; i < 3 ; i++) {
        CCSprite* HG = CCSprite::createWithSpriteFrameName(hg[i].c_str());
        HG->setPosition(ccp(size.width*149/1120, size.height*913/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(HG,2,50+i);
    }
    
    //4.把下面的按钮都换了
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
    
    //5.更换成头上的两个模式按钮
    CCSprite* RankListTitle = CCSprite::createWithSpriteFrameName("RankListTitle.png");
    RankListTitle->setPosition(ccp(size.width/2, size.height*1605/1700+winDif*alpha*alpha));
    layerToRoll->addChild(RankListTitle,4,9);
}