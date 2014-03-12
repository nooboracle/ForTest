#ifndef __knowledgeKing__AsShopLayer__
#define __knowledgeKing__AsShopLayer__
#include "cocos2d.h"
#include "AsMessageBox.h"
using namespace cocos2d;
using namespace std;

class AsShopLayer:public CCLayer{
public:
    //1.适配参数
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    //2.记录菜单点击
    bool m_bTouchedMenu;

    //3.控件
    AsMessageBox* box;
    CCLayerColor* cover;
    CCMenu* shopMenu;
    
    static AsShopLayer* createShopLayer();
    virtual bool showShopLayer();
    
    //shop
    void closeShopLayer();
    void buyGold(CCObject* sender);

    CREATE_FUNC(AsShopLayer);

    void onEnter();
    void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};



#endif /* defined(__knowledgeKing__AsShopLayer__) */
