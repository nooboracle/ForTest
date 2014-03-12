#include "cocos2d.h"
#include "cocos-ext.h"
#include "ASUser.h"
#include "AsMessageBox.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
class LogInScene : public CCLayer,public CCEditBoxDelegate{
public:
    
    virtual bool init();
    static cocos2d::CCScene* scene();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    //用户账户信息
    string UserID;
    string UserNameStr;
    string PassWordStr;
    string UserTokenStr;
    int firstHero = -1;
    string gender = "";
    string nickName = "";
    
    //一些基本层
    CCLayer* AccountLayer;
    AsMessageBox* box;
    
    //基础ui的各种控件
    CCSprite* MainBG;
    CCSprite* grassLand;
    CCSprite* heros;
    CCSprite* lightCircle;
    CCSprite* logo;
    CCSprite* cloud[5];
    CCSprite* thiefKing[3];
    CCSprite* Hero[7];
    
    //1.UI
    void BasicUI();
    void CloudUI();
    void ThiefKing();
    void BeginRandomJumpOut();
    void randomJumpOut();
    
    //2.popUpMenu
    CCLayer* popUpMenuLayer;
    int popUpMenuCounter = 1;
    void popUpMenuDraw();
    void popUpMenuMove();

    //3.call back
    void goToIntroduction();
    void openAccountManagementLayer();
    void closeThisLayer(CCNode* sender);
    void goToTibaoSelection();
    void LogOut();
    
    //2.登录、注册的木板及控件
    void openSignUpTable(CCNode* sender);
    void openBindingTable(CCNode* sender);
    void openChangeAccountTable(CCNode* sender);
    void LogInTableDraw(int _index);
    void closeLogInTable();
    
    void openTextField(CCNode* sender);
    void beginWithOnlineMode();
    void BeginMenuFallDown();
    void MenuMoveOut();
    void removeThis(CCNode* sender);
    
    //3.HTTP方法
    bool boardNotFound = false;
    int tmpBoardID = 0;
    void LogInHTTP();
    void LogInRequestCompleted(CCNode* sender,void* data);
    void SignUpHTTP();
    void SignUpRequestCompleted(CCNode* sender,void* data);
    void getUserHistoryGame(int _index);
    void getUserHistoryGameRequestCompleted(CCNode* sender,void* data);
    void bindUserAccountHTTP();
    void bindUserAccountRequestCompleted(CCNode* sender,void* data);
    void printJson(std::vector<char> *buffer);

    //6.数据库
    void saveLogInInformation(string _name,string _password);
    int loadLogInInformation();

    CREATE_FUNC(LogInScene);
    
    virtual void editBoxEditingDidBegin(CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(CCEditBox *editBox);
    virtual void editBoxTextChanged(CCEditBox *editBox,const std::string &text);
    virtual void editBoxReturn(CCEditBox *editBox);
};


