#include "ASGame.h"
#include "ASBot.h"
#include "GameBaseClass.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace CocosDenshion;

extern ASBot* bot2;
extern ASUser* MainUser;

//乌云被吹走
void ASGame::blowCloudAway(){
    
    if (clouded) {
        
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("blower.wav", false);
        
        bot2->unschedule(schedule_selector(ASBot::countCloudTime));
        cloudTime = 0;
        clouded = false;
        
        //2.play a wind sound here
        
        //3.飞走
        bot2->cloudMoveOut();
        
        if(((GameBaseClass*)getParent())->isIntroducing)
            ((GameBaseClass*)getParent())->nextStep();
    }
}
