#include "ASGame.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace CocosDenshion;

extern ASUser* MainUser;

void ASGame::timeCounter(){
    
    timeLitmit = 20;
    
    for (int i = 1 ; i < 5 ; i ++) {
        CCLabelTTF* clock = CCLabelTTF::create(int2string(0).c_str(),"Arial Rounded MT bold",30);
        clock->setColor(ccc3(0,0,0));
        clock->setPosition(ccp(size.width*375/1000 + size.width*i*101/2000,size.height*17/48+winDif*2));
        addChild(clock,4,playerNumber*10000000+50 + i);
    }
    schedule(schedule_selector(ASGame::timeIsUp),0.04);
}

void ASGame::timeIsUp(){
    
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
        CCLabelTTF* clock = (CCLabelTTF*)this->getChildByTag(playerNumber*10000000+50 + i);
        if( i == 1 || i == 2){     clock->setString(secStr.substr(i-1,1).c_str());   }
        else if (i == 3 || i == 4) {     clock->setString(milstr.substr(i-3,1).c_str());     }
    }
    
    //4.time is up alert
    if (timeLitmit == 3 || timeLitmit == 2 || timeLitmit == 1) {
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("timeisup.mp3");
    }
    
    else if (timeLitmit <= 0){
        
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("wrong.wav",false);
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
        changeHeart(-maxShengMing*0.2,2);
        
        //5.如果有hot
        if (hotRound > 0)
            hotRound--;
        
        //6.如果有dot
        if(dotRound > 0){
            dotRound--;
            changeHeart(-1000-33*(Level-1),2);
            if (dotRound == 0)
                removeChildByTag(1*10000000+102);
        }
    }
}

