#include "ASBot.h"
#include "global.h"
#include <fstream>
#include<iostream>
#include "stdio.h"
#include "stdlib.h"
#include <sys/timeb.h>

extern int tibaoType;
extern bool showed;
extern string tibaoName[100];
extern int djMaxUseNumber[7];

extern string qLabel[100][5000];
extern string optionOne[100][5000];
extern string optionTwo[100][5000];
extern string picFileName[100][5000];
extern int qType[100][5000];
extern int qAnswer[100][5000];

ASBot* ASBot::createBot(int _gameModeTag,int _playerNumber,int _itemLV,int _answerLV,int _roleTag,int _roleLevel ,string _botNickName,string _botID,string _botToken,bool _intro,int _botRank,int _botBaseRankScore,int _botGold,int* _botAllHeroLevel){
    ASBot *pRet = new ASBot();
    if (pRet){
        if (!_intro)
            pRet->initBot(_gameModeTag,_playerNumber,_itemLV,_answerLV,_roleTag,_roleLevel,_botNickName,_botID,_botToken,_botRank,_botBaseRankScore,_botGold,_botAllHeroLevel);
        else
            pRet->IntroBotGameInit();
        return pRet;
    }
    return NULL;
}

bool ASBot::initBot(int _gameModeTag,int _playerNumber,int _itemLV,int _answerLV,int _roleTag,int _roleLevel ,string _botNickName,string _botID,string _botToken,int _botRank,int _botBaseRankScore,int _botGold,int* _botAllHeroLevel){
    //注意：出现随机和固定按照比例决定
    int _propOfConst[5] = {30,40,50,60,80};
    int _randQprob[5] = {50,55,60,65,70};
    int _constQtime_TimeSet[5][20] = {
        {8,9,10,11,12,13,14},
        {7,8,9,10,11,12,13},
        {7,8,9,10,11},
        {6,7,8,9,10},
        {5,6,7,8,9}            
    };
    int _randQtime_R_TimeSet[5][20] = {
        {1,8,9,10,11,12,13,14,15,16},
        {1,7,8,9,10,11,12,13,14},
        {1,7,8,9,10,11,12,13},
        {1,6,7,8,9,10,11},
        {1,6,7,8,9,10},
    };
    
    int _randQtime_W_TimeSet[5][20] = {
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20},
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20},
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20},
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20},
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}
    };
    
    for (int i = 0 ; i < 50 ; i++)
        HeroLevel[i] = -1;
    
    //1.初始化机器人属性
    //1.1 接收特征码
    gameModeTag = _gameModeTag;
    playerNumber = _playerNumber;
    botID = _botID;
    botToken = _botToken;
    itemLV = _itemLV;
    answerLV = _answerLV;
    rolerTag = _roleTag;
    botNickName = _botNickName;
    botRank = _botRank;
    botBaseRankScore = _botBaseRankScore;
    for (int i = 0 ; i < 50 ; i++)
        HeroLevel[i] = _botAllHeroLevel[i];

        //1.2 根据特征码初始化机器人的"答题能力"
    for (int i = 0 ; i < 12 ; i ++) {
        constQtime_TimeSet[i] = _constQtime_TimeSet[answerLV][i];
        randQtime_R_TimeSet[i] = _randQtime_R_TimeSet[answerLV][i];
        randQtime_W_TimeSet[i] = _randQtime_W_TimeSet[answerLV][i];
    }
    randQprob = _randQprob[answerLV];
    propOfConst = _propOfConst[answerLV];
    
    //1.3 随机机器人所带的道具
    struct timeb tp;
    ftime(&tp);
    srand(tp.millitm);
    
    int tmpItemID;
    cout<<"机器人携带的道具编号为 ";
    for (int i = 0 ; i < 3 ; i++) 
        dj_tag[i] = -1;
    for (int i = 0 ; i < 3; i++) {
        do{
            tmpItemID = rand()%6+1;
        }while (itemExists(tmpItemID) || tmpItemID == 3);
        dj_tag[i] = tmpItemID;
        dj_number[i] = djMaxUseNumber[tmpItemID-1];
        cout<<dj_tag[i]<<" 可是使用"<<dj_number[i]<<"次"<<endl;
    }
    cout<<endl;
    
    //2.初始化英雄属性
    Level = _roleLevel;
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
    moveCountNeeded = moveToFight;
    canUseItem = true;
    
    cout<<"初始化后的结果为:"<<endl;
    cout<<"机器人id为:"<<botID<<endl;
    cout<<"机器人的token为"<<botToken<<endl;
    cout<<"机器人的answer等级为"<<answerLV<<endl;
    cout<<"机器人的道具能力等级为"<<itemLV<<endl;
    cout<<"机器人的昵称为"<<botNickName<<endl;
    cout<<"机器人的rank分为"<<botRank<<endl;
    cout<<"机器人的rank基础分为"<<botBaseRankScore<<endl;
    cout<<"机器人使用的英雄编码为"<<rolerTag<<endl;
    cout<<"机器人使用的英雄等级为"<<Level<<endl;
    cout<<"机器人的金币为"<<gold<<endl;
    cout<<"五个英雄的等级为"<<HeroLevel[0]<<" "<<HeroLevel[1]<<" "<<HeroLevel[2]<<" "<<HeroLevel[3]<<" "<<HeroLevel[4]<<endl;

    //3.读取题包
    //3.1 重置数组
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
    
    //3.2 读取txt
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

    //4.初始化
    timeLitmit = 20;
    for (int j = 0;  j < wholeQuestionNumber ; j++ ) {     questionRandomed[j] = false;    }
    showed = false;
    gameRound = 1;
    combo = 0;
    MaxCombo = 0;
    wrongNumber = 0;
    rightNumber = 0;
    clouded = false;
    immune = false;
    
    return true;
}

bool ASBot::itemExists(int _itemID){
    for (int i = 0 ; i < 3; i++) {
        if (dj_tag[i] == _itemID) {
            return true;
        }
    }
    return false;
}