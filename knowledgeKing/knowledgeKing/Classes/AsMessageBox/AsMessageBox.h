#ifndef __knowledgeKing__AsMessageBox__
#define __knowledgeKing__AsMessageBox__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class AsMessageBox:public CCLayer{
public:
    //1.适配参数
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    //2.记录菜单点击
    bool m_bTouchedMenu;
    int type;
    int subType;
    
    //3.控件
    CCLayerColor* cover;
    CCSprite* showbox;
    CCLabelTTF* message;
    int headCounter = 0;
    CCSprite* littleHead[3];
    CCMenu* closeMenu;
    
    static AsMessageBox* createMessageBox(string _message,int _type,int _subType);
    
    //type:1 loading
    void initLoadingBox(string _message);
    void RollHead();
    void LoadingTimeCounter();
    int LoadingCounter = 0;
    //type:2 message + button
    void initMessageBox(string _message,int _subType);
    void addConfirmButton();
    void addGoToShopButton();
    void addBuyEnergyButton();
    void buyEnergy();
    //typ3:3 share
    void initShareMessageBox(int subType);
    void goShare(CCNode* sender);
    
    //type:5 fight info
    void initFightInfo(string _message);
    void addFightMessage(string _message,int colorIndex);

    //General
    void changeMessage(string _message);
    void closeMessageBox();
    void removeThis(CCNode* sender);
    
    CREATE_FUNC(AsMessageBox);
    

    void onEnter();
    void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    
};

#endif /* defined(__knowledgeKing__AsMessageBox__) */
