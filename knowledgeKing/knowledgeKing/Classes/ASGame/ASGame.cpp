#include "ASGame.h"
#include "ASUser.h"
#include <iostream>
#include <fstream>
#include "stdio.h"
#include "stdlib.h"

extern ASUser* MainUser;
extern string tibaoName[100];
extern bool showed;
extern int tibaoType;
extern string imageToRemove[1000];
extern int imageToRemoveCounter;

extern string qLabel[100][5000];
extern string optionOne[100][5000];
extern string optionTwo[100][5000];
extern string picFileName[100][5000];
extern int qType[100][5000];
extern int qAnswer[100][5000];

ASGame* ASGame::createGame(int _gameModeTag,int _playerNumber,bool _intro){
    ASGame *pRet = new ASGame();
    if (pRet){
        if (!_intro)
            pRet->initGame(_gameModeTag,_playerNumber);
        else
            pRet->IntroMainGameInit();
        return pRet;
    }
    return NULL;
}

bool ASGame::initGame(int _gameModeTag,int _playerNumber){

    gameModeTag = _gameModeTag;  //1 is pure Q&A ; 2 is game with property
    playerNumber = _playerNumber;  // 1 is the main player ; 2 is the bot now
    
    //1.初始化英雄属性
    rolerTag = MainUser->HeroSelected;
    //1.0 初始化等级
    Level = MainUser->HeroLevel[rolerTag-1];
    //1.1 基础攻击力
    int attackIndex[6] = {800,600,700,900,1000,200};
    int attackPlus[6] = {20,15,25,25,20,50};
    basicAttack = attackIndex[rolerTag-1] + attackPlus[rolerTag-1]*(Level-1);
    int heartIndex[6] = {10000,10000,10000,10000,10000,10000};
    int heartPlus[6] = {200,250,250,200,150,0};
    shengming = heartIndex[rolerTag-1] + heartPlus[rolerTag-1]*(Level-1);
    if(rolerTag == 3 && Level >= 10)
        shengming = (heartIndex[rolerTag-1] + heartPlus[rolerTag-1]*(Level-1))*(1.15 + 0.01*(Level-10));
    maxShengMing = shengming;
    int moveToFightIndex[6] = {5,5,5,5,5,5};
    float moveToFightIndexMinus[6] = {0,0,0,0,0,0};
    moveToFight = moveToFightIndex[rolerTag-1] - moveToFightIndexMinus[rolerTag-1]*(Level-1);
    if (rolerTag == 1 && Level >= 10)
        moveToFight = 4;
    moveCount = 0;
    if (rolerTag == 4 && Level >= 10)
        moveCount = 3 + 0.1*(Level-10);
    

    //2.从txt中读题
    //2.1 重置数组
    wholeQuestionNumber = 0;
    for (int i = 0 ; i < 50000; i++) {
        answer[i] = -1;
        questionLabel[i] = "";
        twoAnswer[i][0] = "";
        twoAnswer[i][1] = "";
        questionType[i] = -1;
        questionImageFileName[i] = "";
        questionSource[i] = 0;
    }
    
    //2.2 读取txt
    int questionCounter = 0;
    if (tibaoType!=0) {
        for (int i = 0 ; qLabel[tibaoType][i]!="" ; i++) {
            questionLabel[i] = qLabel[tibaoType][i];
            twoAnswer[i][0] = optionOne[tibaoType][i];
            twoAnswer[i][1] = optionTwo[tibaoType][i];
            answer[i] = qAnswer[tibaoType][i];
            questionType[i] = qType[tibaoType][i];
            questionImageFileName[i] = picFileName[tibaoType][i];
            questionSource[i] = tibaoType;
            questionCounter++;
        }
        wholeQuestionNumber = questionCounter;
    }else{
        for (int i = 1 ; i < 15 ; i++) {
            questionCounter = 0;
            for (int j = wholeQuestionNumber; qLabel[i][questionCounter]!=""; j++) {
                questionLabel[j] = qLabel[i][questionCounter];
                twoAnswer[j][0] = optionOne[i][questionCounter];
                twoAnswer[j][1] = optionTwo[i][questionCounter];
                answer[j] = qAnswer[i][questionCounter];
                questionType[j] = qType[i][questionCounter];
                questionImageFileName[j] = picFileName[i][questionCounter];
                questionSource[j] = i;
                wholeQuestionNumber++;
                questionCounter++;
            }
        }
    }
    

    for (int i = 0 ; i < wholeQuestionNumber ; i++) {
        cout<<i<<":"<<questionLabel[i]<<" "<<twoAnswer[i][0]<<" "<<twoAnswer[i][1]<<" "<<questionType[i]<<endl;
    }

    //2.重置数据
    for (int j = 0;  j < wholeQuestionNumber ; j++ ) {     questionRandomed[j] = false;    }
    timeLitmit = 20;
    showed = false;
    gameRound = 1;
    combo = 0;
    MaxCombo = 0;
    wrongNumber = 0;
    rightNumber = 0;
    clouded = false;
    immune = false;
    for (int i = 0 ; i < 1000; i++) {   imageToRemove[i] = "";  }
    imageToRemoveCounter = 0;
    
    return true;
}