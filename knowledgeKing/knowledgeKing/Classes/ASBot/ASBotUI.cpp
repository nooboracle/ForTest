#include "ASBot.h"
#include "ASGame.h"
#include "global.h"
#include "GameBaseClass.h"
using namespace std;

extern ASGame* player1;

void ASBot::UIDraw(){
    
    //2.名字
    CCLabelTTF* name = CCLabelTTF::create("冉默","Arial",30);
    name->setPosition(ccp(size.height*12/13,size.height/20+winDif*2*alpha*alpha));
    //addChild(name,4);
    
    //3.主框
    CCSprite* infoFrame = CCSprite::createWithSpriteFrameName("playerInfoFrame.png");
    infoFrame->setScaleX(-1);
    infoFrame->setPosition(ccp(size.width*31/40, size.height*7/90+winDif*2*alpha*alpha));
    addChild(infoFrame,3);
    
    //4.生命条
    botHeartBar();
    
    //5.名字
    string infoStr = botNickName + " LV." + int2string(Level);
    CCLabelTTF* userName = CCLabelTTF::create(infoStr.c_str(),"Arial Rounded MT Bold",25,CCSizeMake(size.width/2, 0),kCCTextAlignmentRight);
    userName->setPosition(ccp(size.width*9.4/13,size.height*14.25/90+winDif*2.9*alpha));
    addChild(userName,4);
    
    //7.行动力条
    energyBarAnimation();
}

void ASBot::goToNextRound(){
    
    //1.下一轮
    gameRound++;
    
    if(!((GameBaseClass*)getParent())->isIntroducing){
        botChooseToEnterFightLayer();
        botIsThinkingAboutAnswer();
    }
}

void ASBot::removeThis(CCNode* sender){removeChild(sender,true);}




