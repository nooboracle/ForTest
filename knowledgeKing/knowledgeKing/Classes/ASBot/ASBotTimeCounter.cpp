#include "ASBot.h"
#include "global.h"
#include "IOSBridge.h"
#include "GameBaseClass.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

extern GameBaseClass* GameMainLayer;

void ASBot::timeCounter(){
    
    timeLitmit = Easy_Time;
    
    for (int i = 1 ; i < 5 ; i ++) {
        CCLabelBMFont* clock = CCLabelBMFont::create(int2string(0).c_str(),"ziti.fnt");
        clock->setScale(1.6);
        clock->setPosition(ccp(size.width*403/1000 + size.width*i*79/2000,size.height*4/48));
        addChild(clock,4,playerNumber*10000000+50 + i);
    }
    schedule(schedule_selector(ASBot::timeIsUp),0.04);
}

void ASBot::timeIsUp(){
    
    //1.time left
    timeLitmit -= 0.04;
    int sec = (int)timeLitmit;
    int mil = (int)((timeLitmit - (int)timeLitmit)*100);
    
    //2.transfer int to four single strings
    string secStr,milstr;
    if (sec < 10) {  secStr = "0" + int2string(sec);  }
    else{   secStr = int2string(sec);    }
    if (mil < 10) {  milstr = "0" + int2string(mil);  }
    else{   milstr = int2string(mil);   }
    
    //3.set string to the clock
    for (int i = 1 ; i < 5; i++) {
        CCLabelBMFont* clock = (CCLabelBMFont*)this->getChildByTag(playerNumber*10000000+50 + i);
        if( i == 1 || i == 2){     clock->setString(secStr.substr(i-1,1).c_str());   }
        else if (i == 3 || i == 4) {     clock->setString(milstr.substr(i-3,1).c_str());     }
    }
    
    //4.time is up alert
    if (timeLitmit == 3 || timeLitmit == 2 || timeLitmit == 1) {    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("dududu.mp3");     }
    
    else if (timeLitmit <= 0){
        
        //1.下一道
        goToNextRound();
        //2.断combo
        combo = 0;
        //3.减行动力
        if (moveCount) {
            moveCount--;
            changeMoveCount();
            changeEnergyBarAnimation(-100/moveToFight);
        }
        //4.减血
        changeHeart(-2000);
        //5.如果有dot
        if (hotRound > 0)
            hotRound--;
        
        //6.如果有dot
        if(dotRound > 0){
            dotRound--;
            changeHeart(-1000-50*(Level-1));
        }
    }
}

