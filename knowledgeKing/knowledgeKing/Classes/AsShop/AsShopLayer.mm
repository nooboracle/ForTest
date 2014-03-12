#include "AsShopLayer.h"
#include "global.h"
#include "iapGeneral.h"
using namespace std;

int buyIndex = 0;
bool bought = false;
AsShopLayer* shopLayer;

AsShopLayer* AsShopLayer::createShopLayer(){
    shopLayer = new AsShopLayer();
    if (shopLayer){
        shopLayer->showShopLayer();
        return shopLayer;
    }
    return NULL;
}

bool AsShopLayer::showShopLayer(){
    
    //1.黑色底层
    cover = CCLayerColor::create(ccc4(0,0,0,200));
    cover->setPosition(CCPointZero);
    addChild(cover,0);
    
    //2.bg
    CCSprite* shopTable = CCSprite::createWithSpriteFrameName("shop_cover.png");
    shopTable->setPosition(ccp(size.width/2,size.height*109/200+winDif*alpha));
    cover->addChild(shopTable,1);
    
    //4.shop item menu
    string itemFileName[7] = {"gold_l0.png","gold_l1.png","gold_l2.png","gold_l3.png","gold_l4.png","gold_l5.png","gold_l6.png"};
    string itemTouchedFileName[7] = {"gold_l0-s.png","gold_l1-s.png","gold_l2-s.png","gold_l3-s.png","gold_l4-s.png","gold_l5-s.png","gold_l6-s.png"};
    shopMenu = CCMenu::create(NULL);
    for (int i = 1; i < 7; i++) {
        CCSprite* itemSprite = CCSprite::createWithSpriteFrameName(itemFileName[i].c_str());
        CCSprite* itemSprite_s = CCSprite::createWithSpriteFrameName(itemTouchedFileName[i].c_str());
        CCMenuItemSprite* item = CCMenuItemSprite::create(itemSprite,itemSprite_s,this,menu_selector(AsShopLayer::buyGold));
        item->setPosition(ccp(size.width*61/300 + size.width*((i-1)%3)*9/30,size.height*78/110 - size.height*((i-1)/3)*18/70+winDif*alpha));
        shopMenu->addChild(item);
        item->setTag(1000+i);
    }
    shopMenu->setPosition(CCPointZero);
    cover->addChild(shopMenu,2,999);
    
    //5.关闭按钮
    CCSprite* closeSprite = CCSprite::createWithSpriteFrameName("closeMatchTable.png");
    CCSprite* closeSprite_s = CCSprite::createWithSpriteFrameName("closeMatchTable-s.png");
    CCMenuItemSprite* closeItem = CCMenuItemSprite::create(closeSprite,closeSprite_s,this,menu_selector(AsShopLayer::closeShopLayer));
    closeItem->setPosition(ccp(size.width*82/90,size.height*72.5/80+winDif*alpha));
    shopMenu->addChild(closeItem);
    
    //5.Price
    string gold[7] = {"免费金币","3000","9500","16500","38000","98000","198000"};
    int rmbPrice[7] = {0,6,18,30,68,168,328};
    for (int i = 1; i < 7; i++) {
        string priceStr =  "￥"+int2string(rmbPrice[i]);
        CCLabelTTF* goldNumber = CCLabelTTF::create(gold[i].c_str(), "Arial Rounded MT bold", 25);
        CCLabelTTF* price = CCLabelTTF::create(priceStr.c_str(),"Arial Rounded MT bold", 25);
        goldNumber->setColor(ccc3(30,102,129));
        price->setColor(ccc3(30,102,129));
        goldNumber->setPosition(ccp(size.width*61/300 + size.width*((i-1)%3)*9/30,size.height*86.75/110 - size.height*((i-1)/3)*18/70+winDif*alpha));
        price->setPosition(ccp(size.width*56/300 + size.width*((i-1)%3)*9/30,size.height*68/110 - size.height*((i-1)/3)*18/70+winDif*alpha));
        cover->addChild(goldNumber,4);
        cover->addChild(price,4);
    }
    
    return true;
}

void AsShopLayer::buyGold(CCObject* sender){
    
    bought = false;
    
    //1.等候的提示框体
    box = AsMessageBox::createMessageBox("通信中，请耐心等候", 1, 0);
    box->setPosition(CCPointZero);
    addChild(box,1);
    
    //2.调用内购程序
    int type[6] = {TianTianDaRen_Gold_1,TianTianDaRen_Gold_2,TianTianDaRen_Gold_3,TianTianDaRen_Gold_4,TianTianDaRen_Gold_5,TianTianDaRen_Gold_6_1};
    CCMenuItemSprite* itemSelected = (CCMenuItemSprite*)sender;
    buyIndex = itemSelected->getTag() - 1001;
    iapGeneral* seller = [[iapGeneral alloc] init];
    [seller buy:type[buyIndex]];
}

void AsShopLayer::closeShopLayer(){removeFromParentAndCleanup(true);}


void AsShopLayer::onEnter(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
    CCLayer::onEnter();
}

void AsShopLayer::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool AsShopLayer::ccTouchBegan(CCTouch* ptouch,CCEvent* event){
    m_bTouchedMenu = shopMenu->ccTouchBegan(ptouch, event);
    return true;
}

void AsShopLayer::ccTouchMoved(CCTouch* ptouch, CCEvent* event){
    if (m_bTouchedMenu)
        shopMenu->ccTouchMoved(ptouch, event);
}

void AsShopLayer::ccTouchEnded(CCTouch* ptouch, CCEvent* event){
    if (m_bTouchedMenu) {
        shopMenu->ccTouchEnded(ptouch, event);
        m_bTouchedMenu = false;
    }
}
void AsShopLayer::ccTouchCancelled(CCTouch* ptouch, CCEvent* event){
    if (m_bTouchedMenu) {
        shopMenu->ccTouchEnded(ptouch, event);
        m_bTouchedMenu = false;
    }
}


