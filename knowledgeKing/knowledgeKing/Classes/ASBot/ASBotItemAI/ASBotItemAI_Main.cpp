#include "ASBot.h"
#include <sys/timeb.h>

void ASBot::botItemPlan(){

    if (ItemCanBeUsedByBotOrNot(1)!=-1) {   schedule(schedule_selector(ASBot::BotUseCloud),1);   }
    if (ItemCanBeUsedByBotOrNot(2)!=-1) {   schedule(schedule_selector(ASBot::BotUseBlower),1);   }
    //if (ItemCanBeUsedByBotOrNot(3)!=-1) {   schedule(schedule_selector(ASBot::BotUseSkip),1);   }
    if (ItemCanBeUsedByBotOrNot(4)!=-1) {   schedule(schedule_selector(ASBot::botUseAddShengMing),1);   }
    if (ItemCanBeUsedByBotOrNot(5)!=-1) {   schedule(schedule_selector(ASBot::botUseAddMoveCount),1);   }
}

int ASBot::ItemCanBeUsedByBotOrNot(int _itemID){
    //如果携带了该道具，则返回位置序号，否则返回-1
    for (int i = 0 ; i < 3 ; i++) {
        if (dj_tag[i] == _itemID) {   return i;    }
    }
    return -1;
}

void ASBot::BotUseBlower(){
    if (canUseItem) {
        if (dj_number[ItemCanBeUsedByBotOrNot(2)]>0){
            
            if (clouded) {
                struct timeb tp;
                ftime(&tp);
                srand(tp.millitm);
                
                unschedule(schedule_selector(ASBot::BotUseBlower));
                
                int delayTime = rand()%3 + 2;
                CCDelayTime* delay = CCDelayTime::create(delayTime);
                CCCallFunc* blow = CCCallFuncN::create(this,callfuncN_selector(ASBot::blowCloudAway));
                CCDelayTime* delay1 = CCDelayTime::create(2);
                CCCallFunc* reset = CCCallFuncN::create(this,callfuncN_selector(ASBot::resetBotUseBlower));
                CCSequence* seq = CCSequence::create(delay,blow,delay1,reset,NULL);
                runAction(seq);
                
                dj_number[ItemCanBeUsedByBotOrNot(2)]--;
            }
        }
        else
            unschedule(schedule_selector(ASBot::BotUseBlower));
    }
}

void ASBot::resetBotUseBlower(){
    schedule(schedule_selector(ASBot::BotUseBlower),1);
}

void ASBot::botUseAddShengMing(){
    
    if (canUseItem) {
        if (dj_number[ItemCanBeUsedByBotOrNot(4)] > 0) {
            if (shengming <= maxShengMing*0.5) {
                addShengMing();
                dj_number[ItemCanBeUsedByBotOrNot(4)]--;
            }
        }
        else
            unschedule(schedule_selector(ASBot::botUseAddShengMing));
    }
}
