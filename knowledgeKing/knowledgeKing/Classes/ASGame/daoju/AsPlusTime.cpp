#include "ASGame.h"
#include "ASBot.h"
#include "global.h"
#include "IOSBridge.h"
using namespace std;

extern ASBot* bot2;
extern int timeLitmit;

void ASGame::plusClockTime(){
    IOSBridge::PlayAudioIOS("wa");
    
    //1.change time label
    timeLitmit += 5;
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
        
        clock->setColor(ccc3(112,201,72));
        CCActionInterval* fangda = CCScaleTo::create(0.5,2.3);
        CCActionInterval* suoxiao = CCScaleTo::create(0.5,2);
        CCCallFunc* white = CCCallFuncN::create(this,callfuncN_selector(ASGame::LabelBackToWhite));
        CCSequence* seq = CCSequence::create(fangda,suoxiao,white,NULL);
        clock->runAction(seq);
    }
}

