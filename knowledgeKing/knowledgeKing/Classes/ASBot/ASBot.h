#include "cocos2d.h"
#include "ASGame.h"
using namespace cocos2d;
using namespace std;

class ASBot : public CCLayer{
public:
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
//**** part.1 初始化机器人的各种能力 ****//
    
    //机器人特征码
    int gameModeTag;  //游戏类型
    int playerNumber; //主从玩家
    int itemLV;       //使用道具的智能等级
    int answerLV;     //答题能力的智能等级
    string botID;     //机器人id
    string botNickName;   //机器人的名字
    string botToken;  //机器人的Token
    int botRank;      //旧rank
    int botBaseRankScore; //基础rank分
    int gold;         //机器人的金币数
    
    //机器人属性
    int constQnumber; //固定正确题目数
    int randQnumber;  //随机正确题目数
    int constQtime_TimeSet[20];   //答对固定正确题目有可能的时间
    int randQtime_R_TimeSet[20];    //答对随机正确题目有可能的时间
    int randQtime_W_TimeSet[20];    //答错随机正确题目有可能的时间
    int randQprob;            //答对随机题目的概率
    int propOfConst;          //固定正确题目的比例，作为super模式下随机出固定正确题目的概率。
    
    //英雄属性
    int HeroLevel[50];
    int rolerTag;
    int Level;
    int basicAttack;
    int maxShengMing;
    int shengming;
    int moveToFight;
    bool isCritical;
    int damageThisRound;
    
    //机器人行为
    int botAnswer;            //机器人这道题答了哪个答案
    int riskPreference;       //机器人的风险偏好
    int moveCountNeeded;      //机器人倾向于多少行动力释放技能
    void botIsThinkingAboutAnswer();
    void botItemPlan();
    int ItemCanBeUsedByBotOrNot(int _itemID);
    void botChooseToEnterFightLayer();
    void botReadyToEnterFightLayer();
    void botEnterFightLayer();
    void botSelected();
    
    static ASBot* createBot(int _gameModeTag,int _playerNumber,int _itemLV,int _answerLV,int _roleTag,int _roleLevel ,string _botNickName,string _botID,string _botToken,bool _intro,int _botRank,int _botBaseRankScore,int _botGold,int* _botAllHeroLevel);
    virtual bool initBot(int _gameModeTag,int _playerNumber,int _itemLV,int _answerLV,int _roleTag , int _roleLevel ,string _botNickName,string _botID,string _botToken,int _botRank,int _botBaseRankScore,int _botGold,int* _botAllHeroLevel);
    bool itemExists(int _itemID);
    
//**** part.2 游戏的各种属性 ****//    
    
    int rightNumber;
    int wrongNumber;
    int moveCount;
    int gameRound;      //当前游戏轮数
    int qTypeThisRound; //记录该轮的题目是固定正确还是随机正确
    int hotRound;
    int dotRound;
    
    int tmp_shengming;
    
    int calDamage();
    void changeMoveCount();
    void afterFightAnimation();
    void energyBarAnimation();
    void changeEnergyBarAnimation(int _dif);
    void freeFightButton();
    void enterFightLayer();

//**** part.3 道具 ****//

    int dj_tag[3] = {0,0,0};  //该局比赛,选了哪几个道具
    int dj_number[3] = {0,0,0}; //道具可使用次数
    bool clouded = false;  //被云彩了
    bool immune = false;   //对debuff免疫
    bool canUseItem;
    
    float cloudTime;
    float immuneTime;
    
    //1.乌云
    void coverQuestion(int _time);
    void cloudMoveIn(CCSprite* _cloudSprite,CCLabelTTF* _Label);
    void cloudMoveOut();
    void beginCountCloudTime();
    void countCloudTime();
    void removeCloud(CCNode* sender);
    //2.吹风机
    void BotUseBlower();
    void blowCloudAway();
    void resetBotUseBlower();
    //3.跳过本题
    void skipThisQuestion();
    //4.加血
    void botUseAddShengMing();
    void addShengMing();
    //5.加行动力
    void botUseAddMoveCount();
    void addMoveCount();
    //6.加爆击
    float criticalRateAdded = 0;
    int criticalAddedTime;
    void addCriticalRate();
    void CriticalRateCounter();
    //7.按钮到处跑
    void FearButton();
    void ButtonBeginToRun();
    void RandomRunAway();
    
    int RoundBotUseCloud[2];   //同上

    
//**** part.4 技能 ****//

    float silenceTime;
    void dotAnimation();
    void stopDotAnimation();
    void botHotBuff();
    void botHotAnimation();
    void botHotEffected();
    void botSilence();
    void countSilenceTime();
    void botisGangLied();
    void botMinusHpByGangLie();
    
    //3. 乌云
    void goCloud();
    void waitForCloudFinish();
    void BotUseCloud();
    

    int questionIndex;
    float timeLitmit;
    int wholeQuestionNumber;
    int questionType[50000];
    int doubleRoundCounter;
    
    bool questionRandomed[50000];
    int questionRandomedOrder[50000];
    string questionLabel[50000];
    string questionImageFileName[50000];
    string twoAnswer[50000][2];
    int answer[50000];
    int questionSource[50000];

    //2. load data
    void loadData();
    int openTxT(string filePath,int beginIndex,int id);
    
    //3.UI
    void UIDraw();
    void goToNextRound();

    //4. 倒计时
    void timeCounter();
    void timeIsUp();
    void removeThis(CCNode* sender);

    //7. heart
    void botHeartBar();
    void changeHeart(int _shengming,int _source);
    
    //8.Combo
    int combo;
    int MaxCombo;
    
//**** part.4 教学 ****//
    
    bool IntroBotGameInit();
    
    CREATE_FUNC(ASBot);
};