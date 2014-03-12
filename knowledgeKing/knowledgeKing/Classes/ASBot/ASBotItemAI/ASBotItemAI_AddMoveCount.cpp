#include "ASBot.h"

void ASBot::botUseAddMoveCount(){
    
    //1.钢铁侠、sheldon和巫妖王通过行动力药水来更快的发起第一轮攻击
    //2.初音当血低于50%时，通过行动力药水快速的攻击，以恢复生命
    //3.剑圣用行动力药水憋爆，所以应该是在即将攻击之前喝
    //4.初音当生命力足够时，也用行动力药水憋暴
    if (canUseItem) {
        if (dj_number[ItemCanBeUsedByBotOrNot(5)]>0) {
            if ((rolerTag == 0 || rolerTag == 2 || rolerTag == 3 ) && moveToFight - moveCount <= 4) {
                addMoveCount();
                addMoveCount();
                dj_number[ItemCanBeUsedByBotOrNot(5)] = 0;
            }
            else if(rolerTag == 1 && shengming <= 0.5*maxShengMing && moveToFight - moveCount <= 4){
                addMoveCount();
                addMoveCount();
                dj_number[ItemCanBeUsedByBotOrNot(5)] = 0;
            }
        }
        else
            unschedule(schedule_selector(ASBot::botUseAddMoveCount));
    }
}