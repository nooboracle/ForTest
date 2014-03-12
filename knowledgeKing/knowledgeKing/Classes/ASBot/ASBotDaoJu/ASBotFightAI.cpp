#include "ASBot.h"
#include <sys/timeb.h>
#include "GameBaseClass.h"

void ASBot::botReadyToEnterFightLayer(){

    if (moveCount>=moveCountNeeded && !((GameBaseClass*)getParent())->isIntroducing){
        
        //1.如果是js
        if (rolerTag == 5 && dj_number[ItemCanBeUsedByBotOrNot(5)]>0) {
            addMoveCount();
            addMoveCount();
            dj_number[ItemCanBeUsedByBotOrNot(5)] = 0;
        }
        //2.初音还有血
        if (rolerTag == 1 && shengming > maxShengMing*0.5) {
            addMoveCount();
            addMoveCount();
            dj_number[ItemCanBeUsedByBotOrNot(5)] = 0;
        }
        //3.所有人都用暴击
        if(dj_number[ItemCanBeUsedByBotOrNot(6)]>0){
            addCriticalRate();
            dj_number[ItemCanBeUsedByBotOrNot(6)] = 0;
        }
         enterFightLayer();
    }
}

void ASBot::botChooseToEnterFightLayer(){
    
    struct timeb tp;
    ftime(&tp);
    srand(tp.millitm);
    
    //1.钢铁侠，好了就放
    if (rolerTag == 1)
        moveCountNeeded = moveToFight;
    
    //2.初音:生命低于80%，好了就放。如果生命高于80%，憋暴击
    if (rolerTag == 2) {
        if (shengming <= maxShengMing*0.8)
            moveCountNeeded = moveToFight;
        else
            moveCountNeeded = rand()%10+10;
    }
    
    //3.巫妖王：好了就放
    if (rolerTag == 3)
        moveCountNeeded = moveToFight;
    
    //4.谢耳朵，如果没有乌云，则随机
    if (rolerTag == 4) {
        //如果有乌云，则好了就放，然后释放乌云。
        if (ItemCanBeUsedByBotOrNot(1)!=-1 && dj_number[ItemCanBeUsedByBotOrNot(1)]>0)
            moveCountNeeded = moveToFight;
        else
            moveCountNeeded = rand()%10+moveToFight;
        
    }
    
    //5.剑圣，根据答题能力憋爆击,前三个等级好了就放，后两个等级至少憋到7才放
    if (rolerTag == 5) {
        if (answerLV == 0 || answerLV == 1 || answerLV == 2)
            moveCountNeeded= moveToFight;
        else if(answerLV == 3 || answerLV == 4)
            moveCountNeeded = rand()%20+moveToFight+2;
    }
}