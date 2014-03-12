#include "ASBot.h"
#include "ASGame.h"
#include <sys/timeb.h>

extern ASGame* player1;

void ASBot::BotUseCloud(){
    
    //1.如果还有乌云的话->如果是sheldon，则等到沉默了对方再释放；
    //2.如果还有乌云的话->如果是巫妖王，则等到对方中dot了再释放；
    //3.如果还有乌云的话->其他英雄，则上来就放
    //4.如果没有乌云了，ai就不再考虑这个道具了
    //5.根据对手的情况还可以继续细化乌云ai
    if (canUseItem) {
        if (dj_number[ItemCanBeUsedByBotOrNot(1)]>0){
            if (rolerTag == 4) {
                if (player1->silenceTime > 0) {
                    
                    struct timeb tp;
                    ftime(&tp);
                    srand(tp.millitm);
                    
                    unschedule(schedule_selector(ASBot::BotUseCloud));
                    
                    int delayTime = rand()%4 + 3;
                    CCDelayTime* delay = CCDelayTime::create(delayTime);
                    CCCallFunc* goCloud = CCCallFuncN::create(this,callfuncN_selector(ASBot::goCloud));
                    CCSequence* seq = CCSequence::create(delay,goCloud,NULL);
                    runAction(seq);
                }
            }
            else if(rolerTag == 3){
                if (player1->dotRound > 0) {
                    
                    struct timeb tp;
                    ftime(&tp);
                    srand(tp.millitm);
                    
                    unschedule(schedule_selector(ASBot::BotUseCloud));
                    
                    int delayTime = rand()%4 + 3;
                    CCDelayTime* delay = CCDelayTime::create(delayTime);
                    CCCallFunc* goCloud = CCCallFuncN::create(this,callfuncN_selector(ASBot::goCloud));
                    CCSequence* seq = CCSequence::create(delay,goCloud,NULL);
                    runAction(seq);
                }
            }
            else{
                
                struct timeb tp;
                ftime(&tp);
                srand(tp.millitm);
                
                unschedule(schedule_selector(ASBot::BotUseCloud));
                
                int delayTime = rand()%4 + 3;
                CCDelayTime* delay = CCDelayTime::create(delayTime);
                CCCallFunc* goCloud = CCCallFuncN::create(this,callfuncN_selector(ASBot::goCloud));
                CCSequence* seq = CCSequence::create(delay,goCloud,NULL);
                runAction(seq);
            }
        }
        
        else
            unschedule(schedule_selector(ASBot::BotUseCloud));
    }
}

void ASBot::goCloud(){
    
    if(!player1->clouded){
        dj_number[ItemCanBeUsedByBotOrNot(1)]--;
        coverQuestion(15);
    }
   
    schedule(schedule_selector(ASBot::waitForCloudFinish),2);
}

void ASBot::waitForCloudFinish(){
    if(!player1->clouded) {
        unschedule(schedule_selector(ASBot::waitForCloudFinish));
        schedule(schedule_selector(ASBot::BotUseCloud),1);
    }
}
