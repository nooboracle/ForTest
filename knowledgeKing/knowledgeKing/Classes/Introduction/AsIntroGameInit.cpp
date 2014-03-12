#include "ASGame.h"

bool ASGame::IntroMainGameInit(){
    
    //1.英雄属性
    gameModeTag = 2;
    playerNumber = 1;
    rolerTag = 1;
    Level = 1;
    basicAttack = 800;
    shengming = 10000;
    maxShengMing = 10000;
    moveToFight = 5;
    moveCount = 4;

    //2.题目信息
    string tmpQuestion[10] = {"","这款游戏的名字叫什么？","你的角色头像位于哪里？","右下角按钮的作用是什么？","现在该按哪里呢？"};
    string tmpAnswer[10][2] = {
        {"",""},
        {"天天答人","天天打人"},
        {"左上角","右上角"},
        {"发动攻击","使用道具"},
        {"右下角","右下角"},
        {"",""}
    };
    
    for (int i = 0 ; i < 10 ; i++){
        questionType[i] = 2;
        answer[i] = 1;
        questionRandomedOrder[i] = i;
        questionLabel[i] = tmpQuestion[i];
        questionSource[i] = -1;
    }

    for (int i = 0 ; i < 10; i++)
        for (int j = 0 ; j < 2 ; j++)
            twoAnswer[i][j] = tmpAnswer[i][j];

    wholeQuestionNumber = 10;

    //3.游戏规则参数
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