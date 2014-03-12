#include "cocos2d.h"
#include "cocos-ext.h"
#include "AsMessageBox.h"
using namespace std;
using namespace cocos2d;
using namespace extension;

class GameBaseClass : public CCLayer{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    AsMessageBox* box;
    CCLayer* introductionLayer; //教学层
    CCLabelTTF* touchToGoOn;    //浮动字幕
    bool isIntroducing = false;
    bool screenCanBeTouched = false;

    //0.比赛技术统计
    int userSuccess = 0;
    bool gameover = false;
    
    int MainNewRank;         //更新后的rank
    int BotNewRank;
    string BotWinTimesHistory;
    float MainRightRate;       //本场正确率
    float BotRightRate;
    int MainMaxCombo;        //最大连对
    int BotMaxCombo;
    int userGoldPlus;        //加的金币
    int botGoldPlus;
    int botLevelUpIndex;     //机器人自动升级的角色编号
    
    //1.开始游戏
    void beginOnlineGame();
    void beginGameIntroduction();
    
    //2.http
    void updateUserWinHTTP();
    void updateBotWinHTTP();
    void updateUserWinRequestCompleted(CCNode* sender, void* data);
    void updateBotWinRequestCompleted(CCNode* sender,void* data);
    void updateUserGoldHTTP();
    void updateBotGoldHTTP();
    void updateUserGoldRequestCompleted(CCNode* sender, void* data);
    void updateBotGoldRequestCompleted(CCNode* sender,void* data);
    void updateBotHeroLevel();
    void updateBotHeroLevelRequestCompleted(CCNode* sender,void* data);
    void releaseRobot();
    void releaseRobotCompleted(CCNode* sender, void* data);
    //void updateUserRankHTTP();
    //void updateBotRankHTTP();
    //void updateUserRankRequestCompleted(CCNode* sender, void* data);
    //void updateBotRankRequestCompleted(CCNode* sender, void* data);
    void printJson(std::vector<char> *buffer);
    
    //3.结果画面
    void GameResult(int res);
    void refreshRankHTTP();
    void CongScene();
    void RankMedalRecognition();
    void MenuAnimation();

    //4.按钮回调
    void backToLevelSelection();
    void shareToSNS();
    void ReplayGame();
    
    //6.教学模式
    int stepCounter = 1;
    int botAnswerCounter = 0;
    void botAnswerQuestion();
    void TouchNotification();
    void flashMessageBox(string _message,CCPoint _pos,ccColor3B _color);
    void nextStep();
    void IntroStep1();
    void IntroStep2();
    void IntroStep3();
    void IntroStep4();
    void IntroStep5();
    void IntroStep6();
    void IntroStep7();
    void IntroStep8();
    void IntroStep9();
    void IntroStep10();
    void IntroStep11();
    void IntroStep12();
    void IntroStep13();
    void IntroStep14();
    void IntroStep15();
    void IntroStep16();
    
    //6.触屏事件
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *touch ,CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch ,CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch ,CCEvent *event);
    
    CREATE_FUNC(GameBaseClass);
};


