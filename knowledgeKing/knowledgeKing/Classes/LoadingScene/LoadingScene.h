#ifndef __knowledgeKing__LoadingScene__
#define __knowledgeKing__LoadingScene__
#include "cocos2d.h"
#include "AsMessageBox.h"
using namespace cocos2d;
using namespace std;

class LoadingScene : public cocos2d::CCLayer{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF* tips;
    CCLabelTTF* percent;
    CCSprite* littleHead[3];
    int headCounter = 0;
    AsMessageBox* box;
    bool serverInfoLoaded = false;
    bool textureLoaded = false;
    
    //1.ui
    void LoadingSceneUI();
    void RollHead();
    
    //2.读取本地账户信息
    static void saveLogInInformation(string _name,string _password);
    static int loadLogInInformation();
    
    //3.登录
    void LogInHTTP();
    void LogInRequestCompleted(CCNode* sender,void* data);
    void FastSignUp();
    void FastSignUpRequestCompleted(CCNode* sender, void* data);
    void printJson(std::vector<char> *buffer);
    bool boardNotFound = false;
    int tmpBoardID = 0;
    void getUserHistoryGame(int _index);
    void getUserHistoryGameRequestCompleted(CCNode* sender,void* data);
    
    //4.Loading
    int TextureTxTListWholeNumber;
    int TextureFileWholeNumber = 0;
    int LoadCounter = 0;
    int ListCounter = 0;
    int openTxT(int index);
    void InitLoadList(int index);  //0是打开app时候的加载 1是进入战斗时的加载
    void removeSomeList(int index);
    void LoadTexture();
    void addCounter();
    
    //5.计时器
    int counter = 0;
    void TimeCounter();
    
    
    CREATE_FUNC(LoadingScene);
};
#endif /* defined(__knowledgeKing__LoadingScene__) */
