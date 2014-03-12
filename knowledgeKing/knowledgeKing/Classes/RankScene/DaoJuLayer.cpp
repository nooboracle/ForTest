#include "RankScene.h"
#include "ASUser.h"
#include "MyMenu.h"
#include "ASGame.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "AsShopLayer.h"
using namespace CocosDenshion;

extern ASUser* MainUser;
int djSelected[3] = {0,0,0};
string djDesc[7] = {"召唤乌云遮盖对手题目(2次)","立即吹走遮挡的乌云(2次)","立即更换一道题目(4次)","立即回复1000生命(2次)","立即增加2点行动力(2次)","30秒内增加暴击几率(1次)"};
int djPrice[7] = {150,100,200,150,200,150,700};
bool djHasBought[7] = {false,false,false,false,false,false,false};

void RankScene::goToDaojuList(){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    
    disableButtons();
    
    CCCallFunc* stableFormerList;
    if (tableIndex == 1) {  stableFormerList= CCCallFuncN::create(this, callfuncN_selector(RankScene::stableRankList)); }
    else if(tableIndex == 2) {  stableFormerList= CCCallFuncN::create(this, callfuncN_selector(RankScene::stableListList)); }
    tableIndex = 3;
    CCActionInterval* firstHalf = CCOrbitCamera::create(0.35, 1, 0, 0, 90, 0, 0);
    CCCallFunc* middle = CCCallFuncN::create(this, callfuncN_selector(RankScene::middleProcessToDaoJuList));
    CCActionInterval* secondHalf = CCOrbitCamera::create(0.35, 1, 0, 270, 90, 0, 0);
    CCCallFunc* resetContent = CCCallFuncN::create(this, callfuncN_selector(RankScene::resetToDaoJuList));
    CCSequence* seq = CCSequence::create(stableFormerList,firstHalf,middle,secondHalf,resetContent,NULL);
    layerToRoll->runAction(seq);
}

void RankScene::middleProcessToDaoJuList(){
        
    //1.去掉上一个表的内容
    ContainerLayer->removeChildByTag(50);
    ContainerLayer->removeChildByTag(51);
    ContainerLayer->removeChildByTag(52);
    for (int i = 0 ; i < 20 ; i++) {
        ContainerLayer->removeChildByTag(160 + i);
        ContainerLayer->removeChildByTag(260 + i);
        ContainerLayer->removeChildByTag(360 + i);
        ContainerLayer->removeChildByTag(460 + i);
        ContainerLayer->removeChildByTag(560 + i);
        ContainerLayer->removeChildByTag(660 + i);
        ContainerLayer->removeChildByTag(760 + i);
    }
    layerToRoll->removeChildByTag(9);
    layerToRoll->removeChildByTag(10);
    CCLabelBMFont* rankName = (CCLabelBMFont*)layerToRoll->getChildByTag(15);
    rankName->setString("");
    
    //2. 写上这一个表的内容
    buyMenu = MyMenu::create();
    for (int i = 0 ; i < 4 ; i++) {
        //1.道具名称
        string itemTexName = "daoju_" + int2string(i+1) + ".png";
        CCSprite* itemImage = CCSprite::createWithSpriteFrameName(itemTexName.c_str());
        itemImage->setScale(0.8);
        itemImage->setPosition(ccp(size.width*180/1120, size.height*915/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(itemImage,4,160+i);
        
        //2.价格
        CCSprite* priceTable = CCSprite::createWithSpriteFrameName("priceTable.png");
        priceTable->setPosition(ccp(size.width*420/1120,size.height*932/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(priceTable,3,260+i);
        
        CCLabelTTF* itemPrice = CCLabelTTF::create(int2string(djPrice[i]).c_str(),"Arial Rounded MT bold",28,CCPoint(size.width,0),kCCTextAlignmentLeft);
        itemPrice->enableShadow(CCSizeMake(size.width/600, -size.height/500), 255, 0.5);
        itemPrice->setPosition(ccp(size.width*920/1120, size.height*932/1120 - size.height*i*63/660+winDif/130*alpha));
        itemPrice->setColor(ccc3(245,245,245));
        ContainerLayer->addChild(itemPrice,4,360+i);
        
        //3.描述
        CCLabelTTF* desc = CCLabelTTF::create(djDesc[i].c_str(), "Arial Rounded MT bold", 25 , CCPoint(size.width,0),kCCTextAlignmentLeft);
        desc->setPosition(ccp(size.width*840/1120, size.height*892/1120 - size.height*i*63/660+winDif/130*alpha));
        desc->setColor(ccc3(30,102,129));
        ContainerLayer->addChild(desc,4,460+i);
        
        //4.购买提示
        if (djHasBought[i]) {
            CCSprite* boughtTag = CCSprite::createWithSpriteFrameName("greenMark.png");
            boughtTag->setPosition(ccp(size.width*130/1120, size.height*945/1120 - size.height*i*63/660+winDif/130*alpha));
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
    
    //4.下面那个横栏
    CCSprite* itemBottom = CCSprite::createWithSpriteFrameName("item_bottom.png");
    itemBottom->setPosition(ccp(size.width/2,size.height*1050/2200+winDif*alpha*alpha));
    layerToRoll->addChild(itemBottom,3,10);
    
    //5.返回按钮
    CCSprite* backButtonSprite = CCSprite::createWithSpriteFrameName("backToRankList.png");
    CCSprite* backButtonSprite_s = CCSprite::createWithSpriteFrameName("backToRankList-s.png");
    CCMenuItemSprite* back = CCMenuItemSprite::create(backButtonSprite,backButtonSprite_s,this,menu_selector(RankScene::djToRankList));
    back->setPosition(ccp(size.width*9/90,size.height*17/18+winDif*alpha*alpha));
    backMenu = CCMenu::create(back,NULL);
    backMenu->setPosition(CCPointZero);
    layerToRoll->addChild(backMenu,5,11);
    
    //6.换上道具列表的顶部
    CCSprite* itemHead = CCSprite::createWithSpriteFrameName("itemHead.png");
    itemHead->setPosition(ccp(size.width/2,size.height*161/170+winDif*alpha*alpha));
    layerToRoll->addChild(itemHead,4,9);
}

void RankScene::resetToDaoJuList(){
    
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
    for(int i = 0 ; i < 6 ; i++){
        CCSprite* RankTable = CCSprite::createWithSpriteFrameName("other_rank.png");
        RankTable->setPosition(ccp(size.width/2, size.height*2230/1120-size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(RankTable,1,20+i);
    }
    DaoJuContent();
    
    //4.重新启用所有按钮
    enableButtons();
}

void RankScene::DaoJuContent(){
   
    buyMenu = MyMenu::create();
    for (int i = 0 ; i < 6 ; i++) {
        //1.tibao image
        string itemTexName = "daoju_" + int2string(i+1) + ".png";
        CCSprite* itemImage = CCSprite::createWithSpriteFrameName(itemTexName.c_str());
        itemImage->setScale(0.8);
        itemImage->setPosition(ccp(size.width*180/1120, size.height*2230/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(itemImage,4,160+i);
        
        //2.价格
        CCSprite* priceTable = CCSprite::createWithSpriteFrameName("priceTable.png");
        priceTable->setPosition(ccp(size.width*420/1120,size.height*2247/1120 - size.height*i*63/660+winDif/130*alpha));
        ContainerLayer->addChild(priceTable,3,260+i);
        
        CCLabelTTF* itemPrice = CCLabelTTF::create(int2string(djPrice[i]).c_str(),"Arial Rounded MT bold",28,CCPoint(size.width,0),kCCTextAlignmentLeft);
        itemPrice->enableShadow(CCSizeMake(size.width/600, -size.height/500), 255, 0.5);
        itemPrice->setPosition(ccp(size.width*920/1120, size.height*2247/1120 - size.height*i*63/660+winDif/130*alpha));
        itemPrice->setColor(ccc3(245,245,245));
        ContainerLayer->addChild(itemPrice,4,360+i);
        
        //3.描述
        CCLabelTTF* desc = CCLabelTTF::create(djDesc[i].c_str(), "Arial Rounded MT bold", 25 , CCPoint(size.width,0),kCCTextAlignmentLeft);
        desc->setPosition(ccp(size.width*840/1120, size.height*2207/1120 - size.height*i*63/660+winDif/130*alpha));
        desc->setColor(ccc3(30,102,129));
        ContainerLayer->addChild(desc,4,460+i);
        
        //4.购买提示
        if (djHasBought[i]) {
            CCSprite* boughtTag = CCSprite::createWithSpriteFrameName("greenMark.png");
            boughtTag->setPosition(ccp(size.width*130/1120, size.height*2260/1120 - size.height*i*63/660+winDif/130*alpha));
            ContainerLayer->addChild(boughtTag,5,560+i);
        }
        
        //5.购买按钮
        CCSprite* buyButtonSprite = CCSprite::createWithSpriteFrameName("buyItemButton.png");
        CCSprite* buyButtonSprite_s = CCSprite::createWithSpriteFrameName("buyItemButton-s.png");
        CCMenuItemSprite* buyItem = CCMenuItemSprite::create(buyButtonSprite,buyButtonSprite_s,this, menu_selector(RankScene::chooseDj));
        buyItem->setScale(0.8);
        buyItem->setPosition(ccp(size.width*930/1120, size.height*2230/1120 - size.height*i*63/660+winDif/130*alpha));
        buyItem->setTag(1001+i);
        buyMenu->addChild(buyItem);
    }
    buyMenu->setPosition(CCPointZero);
    ContainerLayer->addChild(buyMenu,400,660);
}

void RankScene::energyTimeCounter(){
    
    if (nextRecoverySecond > 0) {
        nextRecoverySecond--;
        string nextRecoveryTimStr = int2string(nextRecoverySecond/60) + ":" + int2string(nextRecoverySecond%60);
        
        CCLabelTTF* nextRecoveryTime = (CCLabelTTF*)this->getChildByTag(46);
        if (nextRecoveryTime!=NULL)
            nextRecoveryTime->setString(nextRecoveryTimStr.c_str());
    }else{
        
        unschedule(schedule_selector(RankScene::energyTimeCounter));
        //1.如果满能量了
        if (MainUser->energyNumber >= 4)
            removeChildByTag(46);
        //2.如果还没满，继续算
        else{
            nextRecoverySecond = 900;
            schedule(schedule_selector(RankScene::energyTimeCounter),1);
        }
        //3.把获得的一点能量画上
        CCSprite* energy = CCSprite::createWithSpriteFrameName("heart.png");
        energy->setPosition(ccp(size.width*10/80+MainUser->energyNumber*size.width*12.7/240, size.height*2100/2200+winDif*4*(2-alpha)));
        addChild(energy,6,41+MainUser->energyNumber);
        //4.
        MainUser->energyNumber++;
    }
}

void RankScene::chooseDj(CCNode* sender){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    
    int tmp = ((CCMenuItemSprite*)sender)->getTag()- 1000;
    if (!djHasBought[tmp-1]) {
        //1.如果钱够
        if(MainUser->gold >= djPrice[tmp-1]){
            changeGold(-djPrice[tmp-1]);
            //1.1 记录下来选择了哪个
            bool fulled = true;
            for (int i = 0 ; i < 3 ; i ++) {
                if (djSelected[i] == 0) {
                    djSelected[i] = tmp;
                    fulled = false;
                    
                    //1.1 标记为已购
                    djHasBought[tmp-1] = true;
                    
                    CCSprite* boughtTag = CCSprite::createWithSpriteFrameName("greenMark.png");
                    boughtTag->setPosition(ccp(size.width*130/1120, size.height*2260/1120 - size.height*(tmp-1)*63/660+winDif/130*alpha));
                    ContainerLayer->addChild(boughtTag,5,560+tmp-1);
                    
                    //1.2 取消按钮
                    CCMenu* buyMenu = (CCMenu*)ContainerLayer->getChildByTag(660);
                    CCMenuItemImage* item = (CCMenuItemImage*)buyMenu->getChildByTag(tmp+1000);
                    CCSpriteFrame* unbuyItem = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("unbuyItemButton.png");
                    CCSpriteFrame* unbuyItem_s = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("unbuyItemButton-s.png");
                    item->setNormalSpriteFrame(unbuyItem);
                    item->setSelectedSpriteFrame(unbuyItem_s);
                    
                    break;
                }
            }
            //1.2 如果已经带了3个道具了
            if (fulled) {
                MessageBox = AsMessageBox::createMessageBox("一场比赛最多只能携带三种道具噢～",2,1);
                MessageBox->setPosition(CCPointZero);
                addChild(MessageBox,100);
            }
        }
        //2.如果钱不够
        else{
            MessageBox = AsMessageBox::createMessageBox("您的金币不足，可以前往商店购买",2,2);
            MessageBox->setPosition(CCPointZero);
            addChild(MessageBox,100);
        }
    //2.购买按钮变为取消
    }else{
        ContainerLayer->removeChildByTag(560+tmp-1);
        djHasBought[tmp-1] = false;
        for (int i = 0 ; i < 3; i++) {
            if (djSelected[i] == tmp) {
                djSelected[i] = 0;
                changeGold(djPrice[tmp-1]);
                break;
            }
        }
        
        //1.2 购买按钮
        CCMenu* buyMenu = (CCMenu*)ContainerLayer->getChildByTag(660);
        CCMenuItemImage* item = (CCMenuItemImage*)buyMenu->getChildByTag(tmp+1000);
        CCSpriteFrame* buyItem = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("buyItemButton.png");
        CCSpriteFrame* buyItem_s = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("buyItemButton-s.png");
        item->setNormalSpriteFrame(buyItem);
        item->setSelectedSpriteFrame(buyItem_s);
    }
}

void RankScene::buyEnergy(){
    if (MainUser->gold >= 120) {
        goldProcessingIndex = 1;
        changeGold(-120);
        updateUserGold();
    }else{
        MessageBox->closeMessageBox();
        MessageBox = AsMessageBox::createMessageBox("您的金币不足\n可以前往商店购买",2, 2);
        MessageBox->setPosition(CCPointZero);
        addChild(MessageBox,1000);
    }
}

void RankScene::showShopLayer(){
    
    MessageBox->closeMessageBox();
    
    AsShopLayer* shopLayer = AsShopLayer::createShopLayer();
    shopLayer->setPosition(CCPointZero);
    addChild(shopLayer,100);
}
