#include "ASBot.h"

bool ASBot::IntroBotGameInit(){
    
    //1.初始化机器人属性
    //1.1 接收特征码
    gameModeTag = 2;
    playerNumber = 2;
    botID = -1;
    botToken = "Introduction";
    itemLV = 0;
    answerLV = 0;
    rolerTag = 1;
    botNickName = "教学炮灰";
    
    //2.初始化英雄属性
    Level = 1;
    basicAttack = 800;
    shengming = 10000;
    maxShengMing = 10000;
    moveToFight = 5;
    moveCount = 7;
    
    for (int i = 0 ; i < 10 ; i++){
        answer[i] = 1;
        questionRandomedOrder[i] = i+1;
    }
    
    //4.初始化
    timeLitmit = 20;
    gameRound = 1;
    combo = 0;
    MaxCombo = 0;
    wrongNumber = 0;
    rightNumber = 0;
    clouded = false;
    immune = false;
    
    return true;
}