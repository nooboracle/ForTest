#include "global.h"
#include "LogInScene.h"
#include "LevelSelectionScene.h"
#include "SimpleAudioEngine.h"
#include "IOSBridge.h"
#include "RankScene.h"
#include "ASUser.h"
#include "MyMenu.h"
using namespace std;
using namespace CocosDenshion;

int tibaoType = 0;
extern bool showed;
extern ASUser* MainUser;
string tibaoName[100] = {"答人大挑战","海贼王","萌喵呆汪","NBA","80后回忆","DotA","魔兽世界","英雄联盟","天文地理","体育竞技","生物科学","生活常识","历史文化","文学艺术","影视娱乐","更多题包"};
string tibaoImageName[100] = {"pAll","pOnePiece","pDog","pNBA","p80","pDota","pWow","plol","pTBC"};
string tibaoBGMName[100] = {"123.mp3","op.mp3","gougou.mp3","nba.mp3","80cartoon.mp3","dota.mp3","wow.mp3","lol.mp3","123.mp3"};

CCScene* LevelSelectionScene::scene(){
    CCScene *scene = CCScene::create();
    LevelSelectionScene *layer = LevelSelectionScene::create();
    scene->addChild(layer);
    return scene;
}

bool LevelSelectionScene::init(){
    if ( !CCLayer::init() ){return false;}
    
    UIDraw();
    scrollViewDraw();
    
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    
    return true;
}

void LevelSelectionScene::scrollViewDraw(){
    
    //0.滑动层
    CCLayer* ContainerLayer = CCLayer::create();
    ContainerLayer->setContentSize(CCSizeMake(24*size.width, winSize.height));
    ContainerLayer->setPosition(CCPointZero);
    ScrollView = CCScrollView::create(CCSizeMake(size.width, winSize.height));
    addChild(ScrollView,1);
    ScrollView->setContainer(ContainerLayer);
    ScrollView->setDelegate(this);
    ScrollView->setDirection(kCCScrollViewDirectionHorizontal);
    ScrollView->setContentOffset(ccp(-tibaoType*640, 0));
    ScrollView->setTouchEnabled(true);

    //1. 题包按钮
    MyMenu* tibaoMenu = MyMenu::create();
    for(int i = 0  ; i < 9 ; i++){
        string item = tibaoImageName[i] + ".png";
        string item_s = tibaoImageName[i] + "-s.png";
        CCSprite* itemSprite = CCSprite::createWithSpriteFrameName(item.c_str());
        CCSprite* itemSprite_s = CCSprite::createWithSpriteFrameName(item_s.c_str());
        CCMenuItemSprite* tibao = CCMenuItemSprite::create(itemSprite,itemSprite_s,this,menu_selector(LevelSelectionScene::tibaoIsSelected));
        tibao->setPosition(ccp(winSize.width/2 + i*winSize.width , winSize.height*280/480));
        tibaoMenu->addChild(tibao);
        tibao->setTag(100+i);
        if (i == 8)
            tibao->setEnabled(false);
    }
    tibaoMenu->setPosition(CCPointZero);
    ContainerLayer->addChild(tibaoMenu,2,7777);

    //2. 题包名称
    string nameStr;
    for (int i = 0 ; i < 9 ; i++) {
        CCSprite* tibaoNameTable = CCSprite::createWithSpriteFrameName("tibaoNameTable.png");
        tibaoNameTable->setPosition(ccp(winSize.width/2 + i*winSize.width , size.height*124.8/130+winDif*2.5*alpha*alpha));
        ContainerLayer->addChild(tibaoNameTable,2);
        
        if (i != 8)
            nameStr = tibaoName[i];
        else
            nameStr = "更多精彩";
        CCLabelBMFont* name = CCLabelBMFont::create(nameStr.c_str(),"puzzlename.fnt");
        name->setScale(0.417);
        name->setPosition(ccp(winSize.width/2 + i*winSize.width,size.height*121.3/130+winDif*2.5*alpha*alpha));
        ContainerLayer->addChild(name,3);
    }
    
    //3. 背景音乐
    if (!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() && !MainUser->muted) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(tibaoBGMName[tibaoType].c_str(),true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.25);
    }

    //4. Rank
    string rankScoreStr;
    for (int j = 0 ; j < 9; j++) {
        CCSprite* plate = CCSprite::createWithSpriteFrameName("maxScoreTable.png");
        plate->setPosition(ccp(winSize.width/2 + j * winSize.width,size.height*40/200+winDif*1.5));
        ContainerLayer->addChild(plate,2);

        CCLabelBMFont* maxscoreLabel = CCLabelBMFont::create(int2string(MainUser->Rank[j]).c_str(), "rank_select.fnt");
        maxscoreLabel->setScale(0.417);
        maxscoreLabel->setPosition(ccp(winSize.width/2 + j * winSize.width,size.height*45/200+winDif*1.5));
        ContainerLayer->addChild(maxscoreLabel,3);
    }
    
    //5. Rank标志
    string medalTex;
    for (int j = 0 ; j < 8; j++) {
        if (MainUser->Rank[j] <= 1200)
            medalTex = "medal_1.png";
        else if(MainUser->Rank[j]>1200 && MainUser->Rank[j] <= 1800)
            medalTex = "medal_2.png";
        else if(MainUser->Rank[j]>1800 && MainUser->Rank[j] <= 2500)
            medalTex = "medal_3.png";
        else if(MainUser->Rank[j]>2500 && MainUser->Rank[j] <= 3500)
            medalTex = "medal_4.png";
        else if(MainUser->Rank[j]>3500 && MainUser->Rank[j] <= 5000)
            medalTex = "medal_5.png";
        else if(MainUser->Rank[j]>5000)
            medalTex = "medal_6.png";
        
        CCSprite* Medal = CCSprite::createWithSpriteFrameName(medalTex.c_str());
        Medal->setScale(0.7);
        Medal->setPosition(ccp(winSize.width*7/24 + j * winSize.width,size.height*55/200+winDif*1.5));
        ContainerLayer->addChild(Medal,3);
    }
}

void LevelSelectionScene::UIDraw(){
    //1.背景
    CCSprite* bg = CCSprite::createWithSpriteFrameName("bg_l.png");
    bg->setPosition(ccp(winSize.width/2,winSize.height/2));
    addChild(bg,0);
    //2.返回及静音按钮
    CCSprite* homeSprite = CCSprite::createWithSpriteFrameName("home.png");
    CCSprite* homeSprite_s = CCSprite::createWithSpriteFrameName("home-s.png");
    CCSprite* muteSprite = CCSprite::createWithSpriteFrameName("muted.png");
    CCSprite* muteSprite_s = CCSprite::createWithSpriteFrameName("muted-s.png");
    CCMenuItemSprite* home = CCMenuItemSprite::create(homeSprite,homeSprite_s,this,menu_selector(LevelSelectionScene::backToMainScene));
    CCMenuItemSprite* mute = CCMenuItemSprite::create(muteSprite,muteSprite_s,this,menu_selector(LevelSelectionScene::muteTheMusic));
    mute->setTag(998);
    home->setPosition(ccp(winSize.width*15/300,winSize.height*289/300));
    mute->setPosition(ccp(winSize.width*303/320,winSize.height*927/960));
    CCMenu* menu = CCMenu::create(home,mute,NULL);
    menu->setPosition(CCPointZero);
    addChild(menu,1,997);
    //3.页码
    CCLabelTTF* indexMark = CCLabelTTF::create("1/9", "Arial Rounded MT bold",30);
    indexMark->setOpacity(200);
    indexMark->setPosition(ccp(winSize.width/2,winSize.height/23));
    addChild(indexMark,2,999);
    //4.左右按钮
    lItemSprite = CCSprite::createWithSpriteFrameName("rightButton.png");
    lItemSprite->setScaleX(-1/2.4);
    lItemSprite->setScaleY(1/2.4);
    rItemSprite = CCSprite::createWithSpriteFrameName("rightButton.png");
    rItemSprite->setScale(1/2.4);
    lItemSprite->setPosition(ccp(size.width/15, winSize.height*3/5));
    rItemSprite->setPosition(ccp(size.width*14/15, winSize.height*3/5));
    rItemSprite->setOpacity(155);
    lItemSprite->setOpacity(155);
    addChild(lItemSprite,1);
    addChild(rItemSprite,1);
    if (tibaoType == 0)
        lItemSprite->setOpacity(0);
    else if(tibaoType == 8)
        rItemSprite->setOpacity(0);
    
    CCActionInterval* scaleUp = CCScaleBy::create(0.5, 1.1);
    CCActionInterval* scaleDown = CCScaleBy::create(0.5, 1/1.1);
    CCSequence* seq = CCSequence::create(scaleUp,scaleDown,NULL);
    CCRepeatForever* effect = CCRepeatForever::create(seq);
    CCActionInterval* scaleUp1 = CCScaleBy::create(0.5, 1.1);
    CCActionInterval* scaleDown1 = CCScaleBy::create(0.5, 1/1.1);
    CCSequence* seq1 = CCSequence::create(scaleUp1,scaleDown1,NULL);
    CCRepeatForever* effect1 = CCRepeatForever::create(seq1);
    lItemSprite->runAction(effect);
    rItemSprite->runAction(effect1);
}

void LevelSelectionScene::scrollViewDidScroll(CCScrollView* view){}
void LevelSelectionScene::scrollViewDidZoom(CCScrollView* view){}
void LevelSelectionScene::onEnter(){CCLayer::onEnter();CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);}
void LevelSelectionScene::onExit(){CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);CCLayer::onExit();}
bool LevelSelectionScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    beginPoint = ScrollView->getContentOffset();
    return true;
}
void LevelSelectionScene::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
void LevelSelectionScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    endPoint = ScrollView->getContentOffset();
    adjustScrollView();
}
void LevelSelectionScene::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    endPoint = ScrollView->getContentOffset();
    adjustScrollView();
}

void LevelSelectionScene::adjustScrollView(){
    if (fabs(endPoint.x-beginPoint.x) > 25/640*winSize.width && endPoint.x > beginPoint.x) {
        LastTibaoType = tibaoType;
        tibaoType--;
        if (tibaoType <= 0){
            tibaoType = 0;
            lItemSprite->setOpacity(0);
        }
        else{
            rItemSprite->setOpacity(155);
            lItemSprite->setOpacity(155);
        }
        if (!MainUser->muted && LastTibaoType!=tibaoType)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(tibaoBGMName[tibaoType].c_str(),true);
        changeIndexMark();
    }
    else if(fabs(endPoint.x-beginPoint.x) > 25/640*winSize.width && endPoint.x < beginPoint.x){
        LastTibaoType = tibaoType;
        tibaoType++;
        if (tibaoType >= 8){
            tibaoType = 8;
            rItemSprite->setOpacity(0);
        }
        else{
            lItemSprite->setOpacity(155);
            rItemSprite->setOpacity(155);
        }
        if (!MainUser->muted  && LastTibaoType!=tibaoType)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(tibaoBGMName[tibaoType].c_str(),true);
        changeIndexMark();
    }
    ScrollView->unscheduleAllSelectors();
    ScrollView->setContentOffsetInDuration(ccp(-tibaoType*winSize.width, 0), 0.3);
}

void LevelSelectionScene::changeIndexMark(){
    CCLabelTTF* indexMark = (CCLabelTTF*)this->getChildByTag(999);
    string str = int2string(tibaoType+1) + "/9";
    indexMark->setString(str.c_str());
}

void LevelSelectionScene::tibaoIsSelected(CCObject* sender){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("book.wav",false);
    tibaoType = ((CCMenuItemFont*)sender)->getTag() - 100;
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, RankScene::scene()));
}

void LevelSelectionScene::backToMainScene(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LogInScene::scene()));
}

void LevelSelectionScene::muteTheMusic(){
    bool tmp = CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
    if (tmp) {
        
        CCMenu* menu = (CCMenu*)this->getChildByTag(997);
        CCMenuItemImage* mute = (CCMenuItemImage*)menu->getChildByTag(998);
        CCSpriteFrame* item1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("unmute.png");
        CCSpriteFrame* item2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ummute-s.png");
        mute->setNormalSpriteFrame(item1);
        mute->setSelectedSpriteFrame(item2);
        
        MainUser->muted = true;
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    }
    else{
        
        CCMenu* menu = (CCMenu*)this->getChildByTag(997);
        CCMenuItemImage* mute = (CCMenuItemImage*)menu->getChildByTag(998);
        CCSpriteFrame* item1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("muted.png");
        CCSpriteFrame* item2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("muted-s.png");
        mute->setNormalSpriteFrame(item1);
        mute->setSelectedSpriteFrame(item2);
        
        MainUser->muted = false;
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(tibaoBGMName[tibaoType].c_str(),true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.3);
    }
}
