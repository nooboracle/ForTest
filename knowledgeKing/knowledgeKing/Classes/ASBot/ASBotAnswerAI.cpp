#include "ASBot.h"
#include "global.h"
#include <sys/timeb.h>
using namespace std;

void ASBot::botIsThinkingAboutAnswer(){
    
    struct timeb tp;
    ftime(&tp);
    srand(tp.millitm);
    //1.随机该轮是“固定正确”，还是“随机正确”
    //0是固定 1是随机
    int index = rand()%100+1;
    if (index <= propOfConst)
        qTypeThisRound = 0;
    else
        qTypeThisRound = 1;

    //2.随机bot会选择哪个答案
    //2.1 设置乌云影响
    int cloudEffect;
    if (qTypeThisRound == 0)
        cloudEffect = 20;
    else if(qTypeThisRound == 1)
        cloudEffect = 30;
    //2.2 随机正确与否
    int rightProb[2] = {100,randQprob};
    int questionNumber = questionRandomedOrder[gameRound];
    int tmp_prob = rand()%100+1;
    if (tmp_prob <= rightProb[qTypeThisRound] - cloudEffect) {
        if(answer[questionNumber] == 0){ botAnswer = 1; }
        else if(answer[questionNumber] == 1){ botAnswer = 0; }
    }
    else if(tmp_prob > randQprob){   botAnswer = answer[questionNumber];    }
    
    //3.随机答题的时间
    int tmp_index;
    int tmp_time;
    if (qTypeThisRound == 0) {
        do{
            tmp_index = rand()%20;
        }while (constQtime_TimeSet[tmp_index]==0);
        tmp_time = constQtime_TimeSet[tmp_index];
    }
    else if(qTypeThisRound == 1) {
        if (botAnswer != answer[questionNumber]) {  //如果答对了
            do{
                tmp_index = rand()%20;
            }while (randQtime_R_TimeSet[tmp_index]==0);
            tmp_time = randQtime_R_TimeSet[tmp_index];
        }
        else{
            do{
                tmp_index = rand()%20;
            }while (randQtime_W_TimeSet[tmp_index]==0);
            tmp_time = randQtime_W_TimeSet[tmp_index];
        }   
    }
    
    //4.倒计时
    schedule(schedule_selector(ASBot::botSelected),tmp_time);
}

void ASBot::botSelected(){
    
    unschedule(schedule_selector(ASBot::botSelected));
    
    int questionNumber = questionRandomedOrder[gameRound];
    //1. 如果答对，则combo＋＋;
    if (botAnswer != answer[questionNumber]){
        
        rightNumber++;
        
        combo++;
        if (combo > MaxCombo) {MaxCombo = combo;}
        
        //2.2 right提示
        CCSprite* right = CCSprite::createWithSpriteFrameName("right.png");
        right->setOpacity(0);
        right->setPosition(ccp(size.width*35.1/40,size.height*7.3/90+winDif*2*alpha*alpha));
        addChild(right,2);
        CCActionInterval* fadeIn = CCFadeIn::create(0.5);
        CCActionInterval* JumpUp = CCJumpBy::create(0.7,ccp(0,0),20,1);
        CCActionInterval* effect = CCSpawn::create(fadeIn,JumpUp,NULL);
        CCActionInterval* fadeOut = CCFadeOut::create(0.3);
        CCCallFunc* remove = CCCallFuncN::create(this,callfuncN_selector(ASGame::removeSprite));
        CCSequence* seq = CCSequence::create(effect,fadeOut,remove,NULL);
        right->runAction(seq);
    
        //2.3 改变行动力
        int movePlus[20] = {1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,5,5};
        int moveToPlus;
        if (combo <= 19)
            moveToPlus = movePlus[combo-1];
        else
            moveToPlus = 5;
        moveCount += moveToPlus;
        changeMoveCount();
        changeEnergyBarAnimation(movePlus[combo-1]*100/moveToFight);
  
        //2.3 如果有hot
        if (hotRound > 0){
            hotRound--;
            botHotAnimation();
            if (hotRound == 0) {
                removeChildByTag(2*10000000+103);
            }
        }
        
        //2.4 如果有dot
        if (dotRound > 0){
            dotRound--;
            if (dotRound == 0)
                removeChildByTag(2*10000000+102);
        }
    }
    
    //3.如果答错
    else{
        
        wrongNumber++;
        
        //3.1 wrong提示
        CCSprite* wrong = CCSprite::createWithSpriteFrameName("wrong.png");
        wrong->setOpacity(0);
        wrong->setPosition(ccp(size.width*35.1/40,size.height*7.4/90+winDif*2*alpha*alpha));
        addChild(wrong,2);
        CCActionInterval* fadeIn = CCFadeIn::create(0.5);
        CCActionInterval* JumpUp = CCJumpBy::create(0.7,ccp(0,0),20,2);
        CCActionInterval* effect = CCSpawn::create(fadeIn,JumpUp,NULL);
        CCActionInterval* fadeOut = CCFadeOut::create(0.3);
        CCCallFunc* remove = CCCallFuncN::create(this,callfuncN_selector(ASGame::removeSprite));
        CCSequence* seq = CCSequence::create(effect,fadeOut,remove,NULL);
        wrong->runAction(seq);
        
        //3.2 减血
        changeHeart(-maxShengMing*0.2,2);
        
        //3.3 如果不在super模式里的话
        combo = 0;
        
        //3.4 改变行动力
        if (moveCount > 0) {
            moveCount--;
            changeMoveCount();
            changeEnergyBarAnimation(-100/moveToFight);
        }
        
        //3.5 如果有hot
        if (hotRound > 0){
            hotRound--;
            if (dotRound == 0)
                removeChildByTag(2*10000000+103);
        }
        
        
        //3.6 如果有dot
        if(dotRound > 0){
            dotRound--;
            changeHeart(-1000-33*(Level-1),2);
            if (dotRound == 0)
                removeChildByTag(2*10000000+102);
        }
    }
    
    if (shengming > 0)
        goToNextRound();
}