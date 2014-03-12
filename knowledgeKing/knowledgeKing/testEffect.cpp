#include "testEffect.h"
#include "ASFightLayer.h"
#include "ASBotFightLayer.h"
#include "global.h"

CCScene* testEffect::scene(){
    CCScene *scene = CCScene::create();
    testEffect *layer = testEffect::create();
    scene->addChild(layer);
    return scene;
}

bool testEffect::init(){
    if ( !CCLayer::init() ){return false;}
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("0.GeneralPart.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("MikuFight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SheldonFight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("IronManFight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("LichKingFight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("BladeMasterFight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("FightGeneral.plist");
    
    ASBotFightLayer* aaa = ASBotFightLayer::createAsFightLayer(2, 4,true,1000,1,10000,6,10000,10);
    aaa->setPosition(CCPointZero);
    addChild(aaa,9999,9999);
    
    CCMenu* menu = CCMenu::create();
    for (int i = 1; i < 7 ; i++) {
        string tmp = "card_" + int2string(i) + ".png";
        CCSprite* repeatSprite = CCSprite::createWithSpriteFrameName(tmp.c_str());
        CCMenuItemSprite* repeat = CCMenuItemSprite::create(repeatSprite,repeatSprite,this,menu_selector(testEffect::Repeat));
        repeat->setScale(0.45);
        repeat->setPosition(ccp(size.width/7 + size.width*(i-1)/7,size.height/8));
        repeat->setTag(i);
        menu->addChild(repeat);
    }
    menu->setPosition(CCPointZero);
    addChild(menu,10000);

    return true;
}

void testEffect::Repeat(CCNode* sender){
    removeChildByTag(9999);
    
    int tmp = ((CCMenuItemSprite*)sender)->getTag();
    
    ASFightLayer* aaa = ASFightLayer::createAsFightLayer(tmp,4,true,1000,1,10000,6,10000,6);
    aaa->setPosition(CCPointZero);
    addChild(aaa,9999,9999);
}

