#include "AsChooseHero.h"
#include "LevelSelectionScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

CCScene* AsChooseHero::scene(){
    CCScene *scene = CCScene::create();
    AsChooseHero *layer = AsChooseHero::create();
    scene->addChild(layer);
    return scene;
}

bool AsChooseHero::init(){
    if ( !CCLayer::init() ){return false;}
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("3.FirstChooseHero.plist");
    chooseHeroUI();
    
    return true;
}

void AsChooseHero::chooseHeroUI(){
    
    //1.背景
    CCSprite* BG = CCSprite::createWithSpriteFrameName("bg_l.png");
    BG->setPosition(ccp(size.width/2, size.height/2+winDif*2));
    addChild(BG,-1);
    
    //2.桌面
    CCSprite* table = CCSprite::createWithSpriteFrameName("bg_FirstChooseHero.png");
    table->setPosition(ccp(size.width/2, size.height/2+winDif*2));
    addChild(table,0);
    
    //3.TextBox
    CCScale9Sprite *nickNameTexture = CCScale9Sprite::createWithSpriteFrameName("FirstHeroTextBox.png");
    CCEditBox *nickNameBox = CCEditBox::create(CCSizeMake(284, 52), nickNameTexture);
    nickNameBox->setPosition(ccp(size.width/2, size.height/5+winDif*2));
    nickNameBox->setFontColor(ccc3(255, 255, 255));
    nickNameBox->setMaxLength(20);
    nickNameBox->setFontName("Arial Rounded MT bold");
    nickNameBox->setFontSize(30);
    nickNameBox->setLabelAnchorPoint(ccp(0.5,0.5));
    nickNameBox->setInputMode(kEditBoxInputModeAny);
    nickNameBox->setInputFlag(kEditBoxInputFlagSensitive);
    nickNameBox->setReturnType(kKeyboardReturnTypeDone);
    nickNameBox->setDelegate(this);
    addChild(nickNameBox,1,12);
    
    //4.男女button
    CCSprite* boyItemSprite = CCSprite::createWithSpriteFrameName("boyButton.png");
    CCSprite* boyItemSprite_s = CCSprite::createWithSpriteFrameName("boyButton-s.png");
    CCSprite* girlItemSprite = CCSprite::createWithSpriteFrameName("girlButton.png");
    CCSprite* girlItemSprite_s = CCSprite::createWithSpriteFrameName("girlButton-s.png");
    CCMenuItemSprite* boyItem = CCMenuItemSprite::create(boyItemSprite,boyItemSprite_s,this,menu_selector(AsChooseHero::selectGender));
    CCMenuItemSprite* girlItem = CCMenuItemSprite::create(girlItemSprite,girlItemSprite_s,this,menu_selector(AsChooseHero::selectGender));
    boyItem->setPosition(ccp(winSize.width*10.8/30, size.height*10.2/30+winDif*2));
    girlItem->setPosition(ccp(winSize.width*21.6/30, size.height*10.2/30+winDif*2));
    boyItem->setTag(6);
    girlItem->setTag(7);
    CCMenu* genderMenu = CCMenu::create(boyItem,girlItem,NULL);
    genderMenu->setPosition(CCPointZero);
    addChild(genderMenu,1);
    
    //5.骰子按钮
    CCSprite* randomItemSprite = CCSprite::createWithSpriteFrameName("randomNameButton.png");
    CCSprite* randomItemSprite_s = CCSprite::createWithSpriteFrameName( "randomNameButton-s.png");
    CCMenuItemSprite* randomItem = CCMenuItemSprite::create(randomItemSprite,randomItemSprite_s,this,menu_selector(AsChooseHero::getRandomNameHTTP));
    randomItem->setPosition(ccp(winSize.width*24/30, size.height*8.3/40+winDif*2));
    CCMenu* randomMenu = CCMenu::create(randomItem,NULL);
    randomMenu->setPosition(CCPointZero);
    addChild(randomMenu,1);
    
    //6.确定按钮
    CCSprite* continueItemSprite = CCSprite::createWithSpriteFrameName("ContinueButton.png");
    CCSprite* continueItemSprite_s = CCSprite::createWithSpriteFrameName("ContinueButton-s.png");
    CCMenuItemSprite* continueItem = CCMenuItemSprite::create(continueItemSprite,continueItemSprite_s,this,menu_selector(AsChooseHero::updateGenderAndNickHTTP));
    continueItem->setPosition(ccp(winSize.width/2,size.height/10+winDif*2));
    CCMenu* continueMenu = CCMenu::create(continueItem,NULL);
    continueMenu->setPosition(CCPointZero);
    addChild(continueMenu,1);
    
    //7.ScrollView
    ContainerLayer = CCLayer::create();
    ContainerLayer->setContentSize(CCSizeMake(2.26*winSize.width, size.height));
    ContainerLayer->setPosition(CCPointZero);
    CCScrollView* ScrollView = CCScrollView::create(CCSizeMake(winSize.width*707/800, winSize.height));
    addChild(ScrollView,1);
    ScrollView->setContainer(ContainerLayer);
    ScrollView->setDelegate(this);
    ScrollView->setBounceable(true);
    ScrollView->isDragging();
    ScrollView->isTouchMoved();
    ScrollView->isBounceable();
    ScrollView->setDirection(kCCScrollViewDirectionHorizontal);
    ScrollView->setPosition(ccp(size.width*48/800, 0));
    
    string heroNameStr[6] = {"钢蛋侠","阿葱音","呜喵王","谢耳朵","剑小圣","草帽飞"};
    string heroTexture[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    string heroDescStr[6] = {
        "高富帅科技控，擅长干扰，快速发动攻击！",
        "百变电音歌手,擅长回复,能够反弹伤害！",
        "北极天灾之主，擅长雪上加霜，高生命值！",
        "天才内核极客，擅长限制，先手发动攻击！",
        "霸气剑道圣者，高伤害，擅长一击必杀！",
        "海上自由王者，耐打抗揍，与敌人死磕到底！"
    };
    string heroType[6] = {"interuptType.png","undeadType.png","undeadType.png","interuptType.png","attackType.png","attackType.png"};
    heroMenu = MyMenu::create();
    for (int i = 0 ; i < 6 ; i++) {
        //1.板子
        CCSprite* heroTableSprite = CCSprite::createWithSpriteFrameName("FirstHeroTable.png");
        CCSprite* heroTableSprite_s = CCSprite::createWithSpriteFrameName("FirstHeroTable-s.png");
        CCMenuItemSprite* heroTable = CCMenuItemSprite::create(heroTableSprite,heroTableSprite_s,this,menu_selector(AsChooseHero::selectFirstHero));
        heroTable->setPosition(ccp(winSize.width*150/800 + winSize.width*i*7.5/20,size.height*2/3+winDif*2));
        heroMenu->addChild(heroTable);
        heroTable->setTag(10+i);
        //2.姓名
        CCLabelTTF* heroName = CCLabelTTF::create(heroNameStr[i].c_str(), "Arial Rounded MT bold", 30);
        heroName->setPosition(ccp(winSize.width*150/800 + winSize.width*i*7.5/20,size.height*58.9/70+winDif*2));
        ContainerLayer->addChild(heroName,3);
        //3.英雄头像
        CCSprite* heroImage = CCSprite::createWithSpriteFrameName(heroTexture[i].c_str());
        heroImage->setScale(0.7);
        heroImage->setPosition(ccp(winSize.width*150/800 + winSize.width*i*7.5/20,size.height*33/50+winDif*2));
        ContainerLayer->addChild(heroImage,3);
        //4.人物介绍
        CCLabelTTF* heroDesc = CCLabelTTF::create(heroDescStr[i].c_str(), "Arial Rounded MT bold", 21,CCSizeMake(size.width/3, 0),kCCTextAlignmentCenter);
        heroDesc->setPosition(ccp(winSize.width*157/800 + winSize.width*i*7.5/20,size.height*49/100+winDif*2));
        heroDesc->setColor(ccc3(149, 70, 15));
        ContainerLayer->addChild(heroDesc,3);
        //5.最高等级
        CCLabelTTF* maxLevel = CCLabelTTF::create("最高30级", "Arial Rounded MT bold", 25);
        maxLevel->setPosition(ccp(winSize.width*150/800 + winSize.width*i*7.5/20,size.height*55.3/70+winDif*2));
        ContainerLayer->addChild(maxLevel,4);
        //6.锁
        if (i == 3 || i == 4 || i == 5) {
            CCSprite* lock = CCSprite::createWithSpriteFrameName("Lock.png");
            lock->setScale(0.8);
            lock->setPosition(ccp(winSize.width*150/800 + winSize.width*i*7.5/20,size.height*33/50+winDif*2));
            ContainerLayer->addChild(lock,4);
        }
        //7.英雄类型
        CCSprite* type = CCSprite::createWithSpriteFrameName(heroType[i].c_str());
        type->setPosition(ccp(winSize.width*56/800 + winSize.width*i*7.5/20,size.height*57.33/70+winDif*2));
        ContainerLayer->addChild(type,4);
    }
    heroMenu->setPosition(CCPointZero);
    ContainerLayer->addChild(heroMenu,2);
}

void AsChooseHero::randomNickName(){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav", false);
    CCEditBox *nickNameBox = (CCEditBox*)this->getChildByTag(12);
    nickNameBox->setText(nickName.c_str());
}

void AsChooseHero::selectGender(CCNode* sender){
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav", false);
    
    removeChild(greenMark);
    int tmp = ((CCMenuItemSprite*)sender)->getTag() - 6;
    if (tmp == 0)
        gender = "male";
    else if(tmp == 1)
        gender = "female";
    CCPoint pos[2] = {CCPoint(winSize.width*5.4/30,size.height*10.17/30+winDif*2),CCPoint(winSize.width*16.25/30,size.height*10.2/30+winDif*2)};
    greenMark = CCSprite::createWithSpriteFrameName("greenMark.png");
    greenMark->setScale(1.5);
    greenMark->setPosition(pos[tmp]);
    addChild(greenMark,2);
}
void AsChooseHero::selectFirstHero(CCNode* sender){
    
    int index = ((CCSprite*)sender)->getTag() - 10;
    
    string attackSound[6] = {"IronMan.mp3","miku.mp3","kuaipao.wav","bazinga.mp3","BM.wav"};
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(attackSound[index].c_str());
    
    if (index == 3 || index == 4) {
        box = AsMessageBox::createMessageBox("该角色只能\n在游戏中解锁", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,10);
    }else if(index == 5){
        box = AsMessageBox::createMessageBox("即将开放\n敬请期待", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,10);
    }else{
        ContainerLayer->removeChild(HeroSelectedMark);
        FirstHeroSelected = index;
        HeroSelectedMark = CCSprite::createWithSpriteFrameName("HeroSelectedMark.png");
        HeroSelectedMark->setPosition(ccp(winSize.width*250/800 + winSize.width*index*7.5/20,size.height*57/100+winDif*2));
        ContainerLayer->addChild(HeroSelectedMark,4);
    }
}

void AsChooseHero::goToTiBaoSelection(){
    unschedule(schedule_selector(AsChooseHero::goToTiBaoSelection));
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LevelSelectionScene::scene()));
}

void AsChooseHero::scrollViewDidScroll(CCScrollView* view){}
void AsChooseHero::scrollViewDidZoom(CCScrollView* view){}
void AsChooseHero::onEnter(){CCLayer::onEnter();CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);}
void AsChooseHero::onExit(){CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);CCLayer::onExit();}
bool AsChooseHero::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){return true;}
void AsChooseHero::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
void AsChooseHero::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}
void AsChooseHero::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){}

void AsChooseHero::editBoxEditingDidBegin(CCEditBox *editBox){nickName = "";}
void AsChooseHero::editBoxEditingDidEnd(CCEditBox *editBox){
    nickName = editBox->getText();
}
void AsChooseHero::editBoxTextChanged(CCEditBox *editBox,const std::string &text){}
void AsChooseHero::editBoxReturn(CCEditBox *editBox){}