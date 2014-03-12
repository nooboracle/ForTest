#include "RankScene.h"
#include "ASUser.h"
#include "GameBaseClass.h"
#include "global.h"
#include "SimpleAudioEngine.h"
using namespace std;
using namespace CocosDenshion;

extern ASUser* MainUser;
int gameMode;

void RankScene::pickListDraw(){
    
    //0.重置一些标识
    roleIndex = 1;
    MainUser->HeroSelected = MainUser->firstHero;
    
    //1.大底座
    CCSprite* table = CCSprite::createWithSpriteFrameName("rolePickTable.png");
    table->setPosition(ccp(size.width/2, size.height/2+winDif*alpha));
    addChild(table,1);
    
    //2.开始按钮
    CCSprite* beginGameItemSprite = CCSprite::createWithSpriteFrameName("beginGameButton.png");
    CCSprite* beginGameItemSprite_s = CCSprite::createWithSpriteFrameName("beginGameButton-s.png");
    CCMenuItemSprite* beginGameItem = CCMenuItemSprite::create(beginGameItemSprite,beginGameItemSprite_s,this,menu_selector(RankScene::BeginGame));
    beginGameItem->setPosition(ccp(size.width/2,size.height/12.7+winDif*alpha));
    beginMenu = CCMenu::create(beginGameItem,NULL);
    beginMenu->setPosition(CCPointZero);
    addChild(beginMenu,2,0);
    
    //3.左右滑动按钮
    CCSprite* lItemSprite = CCSprite::createWithSpriteFrameName("rightButton.png");
    CCSprite* lItemSprite_s = CCSprite::createWithSpriteFrameName("rightButton-s.png");
    CCSprite* rItemSprite = CCSprite::createWithSpriteFrameName("rightButton.png");
    CCSprite* rItemSprite_s = CCSprite::createWithSpriteFrameName("rightButton-s.png");
    CCMenuItemSprite* moveLeftItem = CCMenuItemSprite::create(lItemSprite,lItemSprite_s,this,menu_selector(RankScene::moveLeft));
    moveLeftItem->setScaleX(-0.8);
    moveLeftItem->setScaleY(0.8);
    CCMenuItemSprite* moveRightItem = CCMenuItemSprite::create(rItemSprite,rItemSprite_s,this,menu_selector(RankScene::moveRight));
    moveRightItem->setScale(0.8);
    moveLeftItem->setPosition(ccp(size.width*9/100, size.height*2/9+winDif*alpha));
    moveRightItem->setPosition(ccp(size.width*91/100, size.height*2/9+winDif*alpha));
    moveMenu = CCMenu::create(moveLeftItem,moveRightItem,NULL);
    moveMenu->setPosition(CCPointZero);
    addChild(moveMenu,4,3);
    
    //3.角色信息
    string roleTexture[7] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png","James.png"};
    string roleName[7] = {"钢蛋侠","阿葱音","呜喵王","谢耳朵","剑小圣","草帽飞","？？？？"};
    float scale[7] = {0.8,0.8,0.7,0.72,0.7,0.7,0.7};
    string abilityDesc[7] = {
        "高富帅科技控，擅长干扰，快速发动攻击！",
        "百变电音歌手,擅长回复,能够反弹伤害！",
        "北极天灾之主，擅长雪上加霜，高生命值！",
        "天才内核极客，擅长限制，先手发动攻击！",
        "霸气剑道圣者，高伤害，擅长一击必杀！",
        "海上自由王者，耐打抗揍，与敌人死磕到底！即将开放",
        "更多角色,敬请期待",
    };
    roleScrollLayer = CCLayer::create();
    addChild(roleScrollLayer,2,999);

    LevelUpMenu = CCMenu::create();
    for (int i = 0 ; i < 7 ; i++) {
        //1.角色图片
        CCSprite* role = CCSprite::createWithSpriteFrameName(roleTexture[i].c_str());
        role->setScale(scale[i]);
        role->setPosition(ccp(size.width*174/600 + size.width*i,size.height*101/400+winDif*alpha));
        roleScrollLayer->addChild(role,2);
        //2.名字
        CCLabelTTF* roleNameLabel = CCLabelTTF::create(roleName[i].c_str(), "Arial Rounded MT Bold", 24);
        roleNameLabel->setPosition(ccp(size.width*393/600 + size.width * i, size.height*211/600+winDif*alpha));
        roleScrollLayer->addChild(roleNameLabel,3);
        //3.能力描述
        CCLabelTTF* abilityDescLabel = CCLabelTTF::create(abilityDesc[i].c_str(), "Arial Rounded MT Bold", 22,CCSizeMake(size.width/4,0),kCCTextAlignmentLeft);
        abilityDescLabel->setColor(ccc3(149, 70, 15));
        abilityDescLabel->setPosition(ccp(size.width*6.5/10 + size.width * i , size.height*144/600+winDif*alpha));
        roleScrollLayer->addChild(abilityDescLabel,3);
        //4.角色信息底座
        CCSprite* subTable = CCSprite::createWithSpriteFrameName("roleInfoTable.png");
        subTable->setPosition(ccp(size.width/2 + size.width*i , size.height/2+winDif*alpha));
        roleScrollLayer->addChild(subTable,2);
        //5.等级蓝色底
        CCSprite* infoTable = CCSprite::createWithSpriteFrameName("LevelTable.png");
        infoTable->setPosition(ccp(size.width*18/26 + size.width*i , size.height*188/600+winDif*alpha));
        roleScrollLayer->addChild(infoTable,3);
        //6.等级进度
        CCSprite* LevelBarSprite = CCSprite::createWithSpriteFrameName("LevelBar.png");
        CCProgressTimer* LevelBar = CCProgressTimer::create(LevelBarSprite);
        LevelBar->setPosition(ccp(size.width*18/26 + size.width*i , size.height*188/600+winDif*alpha));
        LevelBar->setType(kCCProgressTimerTypeBar);
        LevelBar->setMidpoint(ccp(0, 0.5));
        LevelBar->setBarChangeRate(ccp(1, 0));
        LevelBar->setPercentage((MainUser->HeroLevel[i]-1)*100/29.);
        roleScrollLayer->addChild(LevelBar,4,1310+i);
        //7.等级
        string LevelStr = "LV." + int2string(MainUser->HeroLevel[i]);
        CCLabelTTF* LevelLabel = CCLabelTTF::create(LevelStr.c_str(), "Arial Rounded MT bold", 20,CCSizeMake(size.width/8,0),kCCTextAlignmentLeft);
        LevelLabel->setPosition(ccp(size.width*14.5/26 + size.width*i, size.height*188.3/600+winDif*alpha));
        roleScrollLayer->addChild(LevelLabel,3,1110+i);
        //8.升级按钮
        if (i!= 5 && i!= 6 && i!=7) {
            string item,item_s;
            if (MainUser->HeroUnLocked[i]) {
                item = "levelUpButton.png";
                item_s = "levelUpButton-s.png";
            }else{
                item = "UnLockHeroButton.png";
                item_s = "UnLockHeroButton-s.png";
            }
            CCSprite* LevelUpItemSprite = CCSprite::createWithSpriteFrameName(item.c_str());
            CCSprite* LevelUpItemSprite_s = CCSprite::createWithSpriteFrameName(item_s.c_str());
            CCMenuItemSprite* LevelUpItem = CCMenuItemSprite::create(LevelUpItemSprite, LevelUpItemSprite_s,this,menu_selector(RankScene::openLevelUpHeroLayer));
            LevelUpItem->setPosition(ccp(size.width*17/26 + size.width*i, size.height*95/600+winDif*alpha));
            LevelUpMenu->addChild(LevelUpItem);
            LevelUpItem->setTag(1010+i);
            
            //9.解锁价格
            if (!MainUser->HeroUnLocked[i]) {
                CCLabelTTF* UnLockPrice = CCLabelTTF::create(int2string(MainUser->HeroUnLockPrice[i]).c_str(), "Arial Rounded MT bold", 24);
                UnLockPrice->setPosition(ccp(size.width*17.3/26 + size.width*i , size.height*96/600+winDif*alpha));
                UnLockPrice->setColor(ccc3(149, 70, 15));
                roleScrollLayer->addChild(UnLockPrice,4,1210+i);
            }
        }
        //10.缩头
        if (!MainUser->HeroUnLocked[i]) {
            CCSprite* lock = CCSprite::createWithSpriteFrameName("Lock.png");
            lock->setScale(0.8);
            lock->setPosition(ccp(size.width*174/600 + size.width*i,size.height*101/400+winDif*alpha));
            roleScrollLayer->addChild(lock,3,1410+i);
        }
    }
    LevelUpMenu->setPosition(CCPointZero);
    roleScrollLayer->addChild(LevelUpMenu,3);
}

void RankScene::BeginGame(CCNode* sender){
    
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    
    if (tableIndex == 3){
        if(!MainUser->HeroUnLocked[roleIndex-1]){
            MessageBox = AsMessageBox::createMessageBox("您还没有\n解锁该角色", 2, 1);
            MessageBox->setPosition(CCPointZero);
            addChild(MessageBox,10001);
        }else{
            if (MainUser->energyNumber > 0) {
                MainUser->HeroSelected = roleIndex;
                goldProcessingIndex = 2;
                updateUserGold();
            }else{
                MessageBox = AsMessageBox::createMessageBox("体力不足\n花费120金币\n就能购买1点体力哦", 2, 4);
                MessageBox->setPosition(CCPointZero);
                addChild(MessageBox,1000);
            }
        }
    }
    else
        goToDaojuList();
}

void RankScene::moveLeft(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    CCLayer* layer = (CCLayer*)this->getChildByTag(999);
    if (roleIndex != 1) {
        CCActionInterval* moveLeft = CCMoveBy::create(1, ccp(size.width, 0));
        CCActionInterval* effect = CCEaseBackInOut::create(moveLeft);
        layer->runAction(effect);
        
        roleIndex--;
    }else{
        if (!lefting) {
            lefting = true;
            CCActionInterval* moveLeft = CCMoveBy::create(0.5, ccp(size.width/9, 0));
            CCActionInterval* effect1 = CCEaseBackIn::create(moveLeft);
            CCActionInterval* moveRight = CCMoveBy::create(1, ccp(-size.width/9,0));
            CCActionInterval* effect2 = CCEaseBackOut::create(moveRight);
            CCCallFunc* reset = CCCallFuncN::create(this, callfuncN_selector(RankScene::resetLeftBool));
            CCSequence* seq = CCSequence::create(effect1,effect2,reset,NULL);
            layer->runAction(seq);
        }
    }
}
void RankScene::moveRight(){
    if(!MainUser->muted)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.wav",false);
    CCLayer* layer = (CCLayer*)this->getChildByTag(999);
    
    if (roleIndex != 7) {
        CCActionInterval* moveRight = CCMoveBy::create(1, ccp(-size.width, 0));
        CCActionInterval* effect = CCEaseBackInOut::create(moveRight);
        layer->runAction(effect);
        
        roleIndex++;
        
    }else{
        if (!righting) {
            righting = true;
            CCActionInterval* moveRight = CCMoveBy::create(0.5, ccp(-size.width/9, 0));
            CCActionInterval* effect1 = CCEaseBackIn::create(moveRight);
            CCActionInterval* moveLeft = CCMoveBy::create(1, ccp(size.width/9,0));
            CCActionInterval* effect2 = CCEaseBackOut::create(moveLeft);
            CCCallFunc* reset = CCCallFuncN::create(this, callfuncN_selector(RankScene::resetRightBool));
            CCSequence* seq = CCSequence::create(effect1,effect2,reset,NULL);
            layer->runAction(seq);
        }
    }
}

void RankScene::resetLeftBool(){    lefting = false;    }
void RankScene::resetRightBool(){   righting = false;   }