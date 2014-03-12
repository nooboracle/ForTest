#include "GameBaseClass.h"
#include "global.h"
#include "ASGame.h"
#include "ASBot.h"
#include <sys/timeb.h>

ASBot* bot2;
extern ASGame* player1;
extern int djSelected[3];
int djMaxUseNumber[7] = {2,2,4,2,2,1};
extern string tmpBotID;
extern string tmpBotName;
extern string tmpBotToken;
extern string tmpBotNickName;
extern int tmpBotAnswerLV;
extern int tmpBotItemLV;
extern int tmpBotHeroID;
extern int tmpBotHeroLevel;
extern int tmpBotRank;
extern int tmpBotBaseRankScore;
extern int tmpBotGold;
extern int tmpBotAllHeroLevel[50];

void GameBaseClass::beginOnlineGame(){
    
    gameover = false;
    
    //1.游戏主背景与上面的板子
    CCSprite* underBG = CCSprite::createWithSpriteFrameName("bg_l.png");
    underBG->setPosition(ccp(size.width/2,size.height/2+winDif*2*alpha*alpha));
    addChild(underBG,0);
    
    CCSprite* upperbg = CCSprite::createWithSpriteFrameName("bg_empty_l.png");
    upperbg->setPosition(ccp(size.width/2,size.height/2+winDif*2*alpha*alpha));
    addChild(upperbg,2,999);
    
    //2.添加主玩家
    player1 =  ASGame::createGame(2,1,false);
    player1->setPosition(CCPointZero);
    addChild(player1,3,99999999);
    
    //2.1 主玩家头像
    string nameStr[6] = {"IronMan.png","Miku.png","LichKing.png","Sheldon.png","BladeMaster.png","Luffy.png"};
    CCSprite* heroImage = CCSprite::createWithSpriteFrameName(nameStr[player1->rolerTag-1].c_str());
    heroImage->setScale(0.5);
    heroImage->setPosition(ccp(size.width*5/40,size.height*80/90+winDif*2*alpha*alpha));
    addChild(heroImage,1,1*10000000+60);
    
    //2.2道具选择情况初始化
    for (int i = 0 ; i < 3 ; i++)
        player1->dj_tag[i] = -1;
    for (int i = 0 ; i < 3 ; i++) {
        if (djSelected[i]!=0) {
            player1->dj_tag[i] = djSelected[i];
            player1->dj_number[i] = djMaxUseNumber[djSelected[i]-1];
        }
    }
    
    //3 附玩家
    bot2 =  ASBot::createBot(2,2,tmpBotItemLV,tmpBotAnswerLV,tmpBotHeroID,tmpBotHeroLevel,tmpBotNickName,tmpBotID,tmpBotToken,false,tmpBotRank,tmpBotBaseRankScore,tmpBotGold,tmpBotAllHeroLevel);
    bot2->setPosition(ccp(0,size.height*8.21/10));
    addChild(bot2,2,99999998);
    
    //3.1 机器人头像
    CCSprite* botheroImage = CCSprite::createWithSpriteFrameName(nameStr[bot2->rolerTag-1].c_str());
    botheroImage->setPosition(ccp(size.width*35.2/40,size.height*80/90+winDif*2*alpha*alpha));
    botheroImage->setScale(0.5);
    addChild(botheroImage,1,2*10000000+60);
    
    
    //4.随机一个题目序列，并赋给这两个玩家
    //4.1 总题目数
    int _wholeQuestionNumber = player1->wholeQuestionNumber;
    
    //4.2 是否已经被随机出来的表识
    bool _questionRandomed[_wholeQuestionNumber];
    for (int i = 0 ; i < _wholeQuestionNumber; i++) {   _questionRandomed[i] = false;     }
    
    //4.3 排序数组
    int _questionIndex;
    
    //4.4 开始随机
    struct timeb tp;
    ftime(&tp);
    std::srand(tp.millitm);
    
    for (int i = 0 ; i < _wholeQuestionNumber; i++) {
        do{
            _questionIndex = rand()%_wholeQuestionNumber;
        }while(_questionRandomed[_questionIndex] == true);
        
        _questionRandomed[_questionIndex] = true;
        player1->questionRandomedOrder[i] = _questionIndex;
        bot2->questionRandomedOrder[i] = _questionIndex;
    }
    
    //4.5 开始游戏
    player1->UIDraw();
    player1->ASdjUI();
    player1->questionDraw();
    player1->rightOrWrongButtonDraw();
    player1->timeCounter();
    
    bot2->UIDraw();
    bot2->botIsThinkingAboutAnswer();
    bot2->botItemPlan();
    bot2->schedule(schedule_selector(ASBot::botReadyToEnterFightLayer),5);
}