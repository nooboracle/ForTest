#include "global.h"
#include "ASGame.h"
using namespace std;

void ASGame::beginWuDi(){
    
    immuneTime = 15;
    immune = true;
    
    wudiAnimation();
    
    CCLabelBMFont* wudiTimeLabel = CCLabelBMFont::create(int2string(immuneTime).c_str(),"ziti.fnt");
    wudiTimeLabel->setPosition(ccp(320,480));
    addChild(wudiTimeLabel,4,playerNumber*10000000+88888);
    
    schedule(schedule_selector(ASGame::wudiTimeCounter),0.98);
}

void ASGame::wudiTimeCounter(){
    immuneTime--;
 
    CCLabelBMFont* wudiTimeLabel = (CCLabelBMFont*)this->getChildByTag(playerNumber*10000000+88888);
    wudiTimeLabel->setString(int2string(immuneTime).c_str());
    
    if (immuneTime <=0) {
        unschedule(schedule_selector(ASGame::wudiTimeCounter));
        WuDiOver();
    }
}

void ASGame::WuDiOver(){
    immune = false;
    removeChildByTag(playerNumber*10000000+88888);
    removeChildByTag(playerNumber*10000000+88889);
}

void ASGame::wudiAnimation(){
    CCLabelTTF* wudiLabel = CCLabelTTF::create("无敌了","Arial",150);
    wudiLabel->setPosition(ccp(320,480));
    addChild(wudiLabel,4,playerNumber*10000000+88889);
}

void ASGame::removeAllDebuff(){
    
    //1. 清乌云
    if (clouded) {      blowCloudAway();    }
    
    //2. 
}