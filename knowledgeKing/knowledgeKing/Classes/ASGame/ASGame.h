#ifndef __KnowledgeKing__ASGame__
#define __KnowledgeKing__ASGame__

#include "cocos2d.h"
#include "AsMessageBox.h"
using namespace cocos2d;
using namespace std;

class ASGame : public CCLayer{
public:
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    AsMessageBox* box;
    CCMenuItemSprite* button1;
    CCMenuItemSprite* button2;
    CCLabelTTF* option1;
    CCLabelTTF* option2;
    
    //1.技术统计
    int gameRound;
    int wrongNumber;
    int rightNumber;
    int combo;
    int moveCount;
    int MaxCombo;
    int heartSoundEffect;

    //2.游戏状态
    int gameModeTag;    //以哪种游戏模式在进行
    int playerNumber;   //玩家编号
    
    //3.生命滚动用
    int tmp_step;
    int tmp_shengming;
    
    //4. 题目属性
    float timeLitmit;
    int moveCoverY;
    int moveCoverX;
    int questionIndex;
    int wholeQuestionNumber;
    int questionType[50000];
    bool questionRandomed[50000];
    int questionRandomedOrder[50000];
    string questionLabel[50000];
    string questionImageFileName[50000];
    string twoAnswer[50000][2];
    int answer[50000];
    int questionSource[50000];
    
    //5. 道具
    int dj_tag[3] = {-1,-1,-1};  //该局比赛,选了哪几个道具
    int dj_number[3] = {}; //道具使用次数
    bool clouded = false;  //被云彩了
    bool immune = false;   //对debuff免疫
    bool rightAnswerShowed = false;  //正确答案
    bool ThiefFollowed = false; //被贼王盯上了
    float cloudTime;
    float immuneTime;
    float silenceTime;
    
    //6. 英雄属性
    int rolerTag;
    int Level;
    int basicAttack;
    int maxShengMing;
    int shengming;
    int moveToFight;
    bool isCritical;
    int damageThisRound;
    
    //7. 战斗表识
    int hotRound;
    int dotRound;
    
    /******************************************************************/
    /******************************************************************/
    
    static ASGame* createGame(int _gameModeTag,int _playerNumber,bool _intro);
    virtual bool initGame(int _gameModeTag,int _playerNumber);
    void loadData();
    int openTxT(string filePath,int beginIndex,int id);
    
    //1.UI
    void UIDraw();
    void goToNextRound();
    void ASdjUI();
    void EggIsSelected(CCNode* sender);
    
    //2.题目
    void questionDraw();
    void rightOrWrongButtonDraw();
    void userSelected(CCObject* sender);
    void timeCounter();
    void timeIsUp();
    void removeSprite(CCObject* sender);
    void movePicCover();
    
    //3.shengming
    void HeartBar();
    void changeHeart(int _shengming,int _source);
    
    //5. 道具
    void djIsSelected(CCObject* sender);
    void changeDaoJuNumber(int _tag);
    //5.1 乌云
    void coverQuestion(int _time);
    void cloudMoveIn(CCSprite* _cloudSprite,CCLabelTTF* _Label);
    void cloudMoveOut();
    void beginCountCloudTime();
    void countCloudTime();
    void removeCloud(CCNode* sender);
    //5.2 吹走乌云
    void blowCloudAway();
    //5.3 跳过本题
    void skipThisQuestion();
    //5.4 加血
    void addShengMing();
    //5.5 加行动力
    void addMoveCount();
    //5.8 加爆击率
    float criticalRateAdded = 0;
    int criticalAddedTime;
    void addCriticalRate();
    void CriticalRateCounter();
    
    //6.技能
    void silence();
    void countSilenceTime();
    void dotAnimation();
    void dotEffected();
    void hotBuff();
    void hotAnimation();
    void hotEffected();
    void userIsGangLied();
    void userMinusHpByGangLie();

    //9.行动力->能量槽->进入战斗->退出战斗
    void changeMoveCount();
    void energyBarAnimation();
    void changeEnergyBarAnimation(int _dif);
    void freeFightButton();
    void enterFightLayer();
    int calDamage();
    void afterFightAnimation();
    
    //教学
    bool IntroMainGameInit();

    CREATE_FUNC(ASGame);
};
#endif
