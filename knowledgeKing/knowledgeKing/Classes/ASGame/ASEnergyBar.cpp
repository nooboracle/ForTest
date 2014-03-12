#include "ASGame.h"
#include "global.h"

void ASGame::energyBarAnimation(){
    
    //1.显示行动力的圆形底座
    CCSprite* energyBall = CCSprite::createWithSpriteFrameName("newEnergyContainer.png");
    energyBall->setPosition(ccp(size.width*17.38/80,size.height*77.85/90+winDif*2*alpha*alpha));
    addChild(energyBall,4);
    
    //2.行动力数量
    string perStr = int2string(moveCount);
    CCLabelTTF* per = CCLabelTTF::create(perStr.c_str(),"Arial Rounded MT bold",32);
    per->setPosition(ccp(size.width*17.38/80,size.height*77.85/90+winDif*2*alpha*alpha));
    addChild(per,5,1*10000000+26);
    
    //3.能量条
    CCSprite* energyBarSprite = CCSprite::createWithSpriteFrameName("newEnergyBar.png");
    CCProgressTimer* energyBar = CCProgressTimer::create(energyBarSprite);
    energyBar->setPosition(ccp(size.width*8.45/80,size.height*79.89/90+winDif*2*alpha*alpha));
    energyBar->setType(kCCProgressTimerTypeRadial);
    energyBar->setPercentage(moveCount*20);
    addChild(energyBar,4,1*10000000+20);
    
    //4.攻击按钮底座
    CCSprite* containerSprite = CCSprite::createWithSpriteFrameName("energyContainer.png");
    CCSprite* containerSprite_s = CCSprite::createWithSpriteFrameName("energyContainer.png");
    CCMenuItemSprite* energyContainer = CCMenuItemSprite::create(containerSprite,containerSprite_s,this,menu_selector(ASGame::enterFightLayer));
    energyContainer->setScale(1.05);
    energyContainer->setPosition(ccp(size.width*179.5/200,size.height*10.25/135));
    CCMenu* energyContainerMenu = CCMenu::create(energyContainer,NULL);
    energyContainerMenu->setPosition(CCPointZero);
    if (moveCount < moveToFight)
            energyContainerMenu->setTouchEnabled(false);
    addChild(energyContainerMenu,2,1*10000000+27);
    
    //5.攻击按钮充值槽
    CCSprite* barSprite = CCSprite::createWithSpriteFrameName("energyBar.png");
    CCProgressTimer* attackButton = CCProgressTimer::create(barSprite);
    attackButton->setScale(1.05);
    attackButton->setPosition(ccp(size.width*179.04/200,size.height*10/135));
    attackButton->setType(kCCProgressTimerTypeBar);
    attackButton->setMidpoint(ccp(0.5, 0));
    attackButton->setBarChangeRate(ccp(0, 1));
    attackButton->setPercentage(100*moveCount/moveToFight);
    addChild(attackButton,3,1*10000000+25);
    
    //4.能量的盖子
    CCSprite* energyBarCover = CCSprite::createWithSpriteFrameName("energyBarCover.png");
    energyBarCover->setPosition(ccp(size.width*179.5/200,size.height*10.25/135));
    energyBarCover->setOpacity(0);
    //addChild(energyBarCover,4,1*10000000+28);
}

void ASGame::changeEnergyBarAnimation(int _dif){
    
    CCProgressTimer* energyBar = (CCProgressTimer*)this->getChildByTag(playerNumber*10000000+20);
    CCProgressTimer* attackButton = (CCProgressTimer*)this->getChildByTag(1*10000000+25);
    int percentage = energyBar->getPercentage();

    //1.上方行动力槽
    CCProgressFromTo* action = CCProgressFromTo::create(0.015*(abs(moveCount*100/moveToFight-percentage)), percentage, moveCount*100/moveToFight);
    energyBar->runAction(action);

    //2.下方攻击按钮槽
    CCProgressFromTo* action1 = CCProgressFromTo::create(0.015*(abs(moveCount*100/moveToFight-percentage)), percentage, moveCount*100/moveToFight);
    attackButton->runAction(action1);
    
    //3.能量条盖子
    /*
     CCSprite* energyBarCover = (CCSprite*)this->getChildByTag(player 28);
     energyBarCover->setOpacity(255);
     float tmpScale = (sqrt(pow(107, 2) - pow((abs((int)(107-_pixel))),2))*2)/214;
     energyBarCover->setScale(tmpScale);
     energyBarCover->setPosition(ccp(size.width*179/200, size.height/13.5 - (215-_pixel)/2*tmpScale));
     */
}