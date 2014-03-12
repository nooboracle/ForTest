#include "RankScene.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern int tibaoType;
extern string tibaoName[100];
extern ASUser* MainUser;

void RankScene::goToRankList(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    
    disableButtons();
    
    tableIndex = 1;
    CCCallFunc* stableListList = CCCallFuncN::create(this, callfuncN_selector(RankScene::stableListList));
    CCActionInterval* firstHalf = CCOrbitCamera::create(0.35, 1, 0, 0, 90, 0, 0);
    CCCallFunc* middle = CCCallFuncN::create(this, callfuncN_selector(RankScene::middleProcessToRankList));
    CCActionInterval* secondHalf = CCOrbitCamera::create(0.35, 1, 0, 270, 90, 0, 0);
    CCCallFunc* resetContent = CCCallFuncN::create(this, callfuncN_selector(RankScene::resetToRankList));
    CCSequence* seq = CCSequence::create(stableListList,firstHalf,middle,secondHalf,resetContent,NULL);
    layerToRoll->runAction(seq);
}

void RankScene::stableListList(){
        
    //1.TODO 获得当前scrollview的位置，这样翻转时候就不会出现列表错位
    
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
    
    //3.3 排行榜内容
    chooseMenu = MyMenu::create();
    for (int i = 0 ; i < 4 ; i++) {
        
        //1.板子
        CCSprite* RankTable = CCSprite::createWithSpriteFrameName("other_rank.png");
        RankTable->setPosition(ccp(size.width/2, size.height*915/1120-size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(RankTable,1,20+i);
        
        //2.题包图片
        CCSprite* userImageItem = CCSprite::createWithSpriteFrameName(userImage[i].c_str());
        userImageItem->setScale(0.5);
        userImageItem->setPosition(ccp(size.width*200/1120, size.height*915/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(userImageItem,4,160+i);
        
        //3.题包名称
        CCLabelBMFont* name = CCLabelBMFont::create(tibaoName[i].c_str(),"listname.fnt",size.width*0/3,kCCTextAlignmentLeft);
        name->setScale(0.417);
        name->setPosition(ccp(size.width*520/1120, size.height*915/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(name,4,260+i);
        
        //4.前往按钮
        CCSprite* chooseButtonSprite = CCSprite::createWithSpriteFrameName("chooseTiBao.png");
        CCSprite* chooseButtonSprite_s = CCSprite::createWithSpriteFrameName("chooseTiBao-s.png");
        CCMenuItemSprite* chooseItem = CCMenuItemSprite::create(chooseButtonSprite,chooseButtonSprite_s,this,menu_selector(RankScene::tibaoRankIsSelected));
        chooseItem->setScale(0.8);
        chooseItem->setPosition(ccp(size.width*940/1120, size.height*915/1120 - size.height*i*63/660+winDif/130*alpha));
        chooseItem->setTag(560+i);
        chooseMenu->addChild(chooseItem);
    }
    chooseMenu->setPosition(CCPointZero);
    chooseMenu->setTouchPriority(0);
    ContainerLayer->addChild(chooseMenu,1,660);
}

void RankScene::middleProcessToRankList(){
    
    //1.去掉上一个表的内容
    for (int i = 0 ; i < 20 ; i++) {
        ContainerLayer->removeChildByTag(160 + i);
        ContainerLayer->removeChildByTag(260 + i);
        ContainerLayer->removeChildByTag(360 + i);
        ContainerLayer->removeChildByTag(460 + i);
        ContainerLayer->removeChildByTag(560 + i);
        ContainerLayer->removeChildByTag(660);
    }
    CCLabelBMFont* rankName = (CCLabelBMFont*)layerToRoll->getChildByTag(15);
    rankName->setString(tibaoName[tibaoType].c_str());
    
    //2.启用三个按钮
    for (int i = 0 ; i < 3 ; i++) {
        CCMenuItemSprite* item = (CCMenuItemSprite*)((layerToRoll->getChildByTag(10))->getChildByTag(12+i));
        item->setEnabled(true);
    }
    
    //3. 皇冠
    string hg[3] = {"goldHG.png","silverHG.png","bronzeHG.png"};
    for (int i = 0 ; i < 3 ; i++) {
        CCSprite* HG = CCSprite::createWithSpriteFrameName(hg[i].c_str());
        HG->setPosition(ccp(size.width*149/1120, size.height*913/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(HG,2,50+i);
    }
    
    //4.写上这一个表的内容
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
}

void RankScene::resetToRankList(){
    
    //1.把containerlayer去了
    layerToRoll->removeChildByTag(998);
    
    //2.重新以scrollview的方式放在上面
    ContainerLayer = CCLayerColor::create(ccc4(0,0,0,0));
    ContainerLayer->setContentSize(CCSizeMake(size.width, size.height*2.14));
    ContainerLayer->setPosition(ccp(0,-size.height*1.675+winDif/130*alpha));

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
    
    //3.把内容都放上
    //3.1 前20名的板子
    for(int i = 0 ; i < 20 ; i++){
        CCSprite* RankTable = CCSprite::createWithSpriteFrameName("other_rank.png");
        RankTable->setPosition(ccp(size.width/2, size.height*2230/1120-size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(RankTable,1,20+i);
    }
    
    //3.2 写上这一个表的内容
    rankListContent();
    
    //4.重新启用所有按钮
    enableButtons();
    
    //5.启用排行榜列表按钮
    //((CCMenuItemSprite*)(bottomMenu->getChildByTag(14)))->setEnabled(true);
}


void RankScene::rankListContent(){
    
    //1.改变结算剩余时间
    string daysToExpire = int2string(serverTime) + "天 后结算本周排名";
    daysLeftLabel->setString(daysToExpire.c_str());
    
    //2.皇冠
    string hg[3] = {"goldHG.png","silverHG.png","bronzeHG.png"};
    for (int i = 0 ; i < 3 ; i++) {
        CCSprite* HG = CCSprite::createWithSpriteFrameName(hg[i].c_str());
        HG->setPosition(ccp(size.width*149/1120, size.height*2228/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(HG,2,50+i);
    }
    
    //3.排行榜信息
    for (int i = 0 ; i < 20 ; i++) {
        //3.1 图片
        CCSprite* userImageItem = CCSprite::createWithSpriteFrameName(userImage[i].c_str());
        userImageItem->setScale(0.5);
        userImageItem->setPosition(ccp(size.width*294/1120, size.height*2230/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(userImageItem,4,160+i);
        
        //3.2 用户名
        if (nickName_Rank[i] == "")
            nickName_Rank[i] = "暂无人上榜";
        CCLabelTTF* name = CCLabelTTF::create(nickName_Rank[i].c_str(), "Arial Rounded MT Bold", 23,CCSizeMake(size.width/2, 0),kCCTextAlignmentLeft);
        name->setColor(ccc3(50, 50, 50));
        name->setPosition(ccp(size.width*655/1120, size.height*2250/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(name,4,260+i);
        
        //3.3 胜率
        CCLabelTTF* winPercentLabel = CCLabelTTF::create("胜率", "Arial Rounded MT Bold",32,CCSizeMake(size.width/5, 0),kCCTextAlignmentLeft);
        winPercentLabel->setColor(ccc3(149, 70, 15));
        winPercentLabel->setPosition(ccp(size.width*815/1120,size.height*2230/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(winPercentLabel,4,760+i);
        
        string weeklyWinRateStr = winPercent_Rank[i];
        if (winPercent_Rank[i] == "")
            weeklyWinRateStr = "0%";
        CCLabelTTF* winPercent = CCLabelTTF::create(weeklyWinRateStr.c_str(), "Thonburi-Bold",32,CCSizeMake(size.width/3, 0),kCCTextAlignmentRight);
        winPercent->setColor(ccc3(28,104,117));
        winPercent->setPosition(ccp(size.width*820/1120, size.height*2231/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(winPercent,4,360+i);

        //3.4 胜场
        CCLabelTTF* winTimesLabel = CCLabelTTF::create("胜场", "Arial Rounded MT Bold", 23,CCSizeMake(size.width/10, 0),kCCTextAlignmentLeft);
        winTimesLabel->setColor(ccc3(149, 70, 15));
        winTimesLabel->setPosition(ccp(size.width*434/1120,size.height*2210/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(winTimesLabel,4,660+i);
        
        string weeklyWinStr;
        if (weeklyWin_Rank[i]!="")
            weeklyWinStr = weeklyWin_Rank[i];
        else
            weeklyWinStr = "0";
        CCLabelTTF* weeklywin = CCLabelTTF::create(weeklyWinStr.c_str(), "Verdana-Bold",24,CCSizeMake(size.width/5, 0),kCCTextAlignmentLeft);
        weeklywin->setColor(ccc3(28,104,117));
        weeklywin->setPosition(ccp(size.width*580/1120, size.height*2209.5/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(weeklywin,4,460+i);
        
        //3.5 名次数字
        if (i > 2) {
            CCLabelBMFont* classnumber = CCLabelBMFont::create(int2string(i+1).c_str(), "classnum.fnt");
            classnumber->setScale(0.417);
            classnumber->setPosition(ccp(size.width*146/1120, size.height*2230/1120 - size.height*i*63/660+winDif/130*alpha));
            ContainerLayer->addChild(classnumber,4,560+i);
        }
    }
}
