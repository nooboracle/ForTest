#include "TuiTuMapScene.h"
#include "MyMenu.h"
#include <sys/timeb.h>
using namespace std;

int bossIndex;
int successRequirementIndex;

void TuiTuMapScene::basicMapDraw(){
    
    CCLayer* ContainerLayer = CCLayer::create();
    ContainerLayer->setContentSize(CCSizeMake(2*size.width, winSize.height));
    ContainerLayer->setPosition(CCPointZero);
    ScrollView = CCScrollView::create(CCSizeMake(size.width, winSize.height));
    addChild(ScrollView,1);
    ScrollView->setContainer(ContainerLayer);
    ScrollView->setDelegate(this);
    ScrollView->setDirection(kCCScrollViewDirectionHorizontal);
    //ScrollView->setTouchEnabled(true);
    
    //1. 题包按钮
    string chapterNameStr[6] = {"科技高塔","青葱农场","冰封王座","天才城市","剑之巅峰","答人竞技场"};
    CCPoint chapterPosition[6] = {ccp(size.width/3,size.height/3),ccp(size.width*2/3, size.height*2/3),ccp(size.width*4/3,size.height/3),ccp(size.width*5/3,size.height*2/3),ccp(size.width*7/3, size.height/3),ccp(size.width*8/3, size.height*2/3)};
    MyMenu* chapterMenu = MyMenu::create();
    for(int i = 0  ; i < 6 ; i++){
        CCMenuItemFont* chapterItem = CCMenuItemFont::create(chapterNameStr[i].c_str(),this,menu_selector(TuiTuMapScene::chapterIsSelected));
        chapterItem->setPosition(chapterPosition[i]);
        chapterMenu->addChild(chapterItem);
        chapterItem->setTag(i);
    }
    chapterMenu->setPosition(CCPointZero);
    ContainerLayer->addChild(chapterMenu,1);
}

void TuiTuMapScene::chapterIsSelected(cocos2d::CCNode *sender){
    bossIndex = ((CCMenuItemSprite*)sender)->getTag();
    openTiBaoLayer();
}

void TuiTuMapScene::openTiBaoLayer(){
    
    //1.禁用下层操作
    ScrollView->setTouchEnabled(false);
    
    //2.选择题包层
    TiBaoLayer = CCLayerColor::create(ccc4(0,0,0,155));
    TiBaoLayer->setPosition(CCPointZero);
    addChild(TiBaoLayer,2);
    
    CCSprite* layer = CCSprite::createWithSpriteFrameName("matchTable.png");
    layer->setPosition(ccp(size.width/2, winSize.height/2));
    TiBaoLayer->addChild(layer,0);
    
    CCSprite* closeSprite = CCSprite::createWithSpriteFrameName("closeMatchTable.png");
    CCSprite* closeSprite_s = CCSprite::createWithSpriteFrameName("closeMatchTable-s.png");
    CCMenuItemSprite* closeItem = CCMenuItemSprite::create(closeSprite,closeSprite_s,this,menu_selector(TuiTuMapScene::removeTiBaoLayer));
    closeItem->setPosition(ccp(winSize.width*6/7,winSize.height*6/7));
    CCMenu* closeMenu = CCMenu::create(closeItem,NULL);
    closeMenu->setPosition(CCPointZero);
    TiBaoLayer->addChild(closeMenu,1);
    
    //3.备选题包的列表
    TiBaoListDraw();
    
    //4.胜利条件
    SuccessRequirementDraw();
}

void TuiTuMapScene::removeTiBaoLayer(CCNode* sender){sender->getParent()->getParent()->removeChild(sender->getParent());}

void TuiTuMapScene::TiBaoListDraw(){
    
}

void TuiTuMapScene::SuccessRequirementDraw(){
    struct timeb tp;
    ftime(&tp);
    std::srand(tp.millitm);
    successRequirementIndex = rand()%6;
    //0是普通战斗 1是boss战 2是一人一道看谁先死 3是看谁先答完10道题 4是极速模式
    
    string ruleStr[5] = {
        "天天答人经典战斗模式,ko对手者胜",
        "",
        "敌我双方轮流答题,答错者减少1/3生命,坚持到最后者胜",
        "双方同时答题,先答对10道题者胜",
        "每题只有5秒时限，坚持道最后者胜"
    };
    
    string bossRuleStr[5] = {
        "钢蛋侠战斗规则",
        "阿葱音战斗规则",
        "呜喵王战斗规则",
        "谢耳朵战斗规则",
        "剑小圣战斗规则"
    };
    
    string str;
    if (successRequirementIndex == 1)
        str = bossRuleStr[bossIndex];
    else
        str = ruleStr[successRequirementIndex];
    
    CCLabelTTF* rule = CCLabelTTF::create(str.c_str(), "Arial Rounded MT Bold", 24);
    rule->setPosition(ccp(size.width/2,size.height/2));
    TiBaoLayer->addChild(rule,1);
}