#include "ASBot.h"
#include "ASGame.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace CocosDenshion;

extern ASGame*player1;
extern ASUser* MainUser;

void ASBot::blowCloudAway(){
    
    if (clouded) {
        
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("blower.wav", false);
        
        player1->unschedule(schedule_selector(ASGame::countCloudTime));
        cloudTime = 0;
        clouded = false;
        
        //2.play a wind sound here
        
        //3.飞走
        player1->cloudMoveOut();

    }
}