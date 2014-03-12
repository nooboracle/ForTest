#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;

class LevelSelectionScene : public CCLayer, public CCScrollViewDelegate{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    CCScrollView* ScrollView;
    CCSprite* lItemSprite;
    CCSprite* rItemSprite;
    CCPoint beginPoint;
    CCPoint endPoint;
    int LastTibaoType;
    
    void UIDraw();
    void scrollViewDraw();
    void tibaoIsSelected(CCObject* sender);
    void changeIndexMark();
    void adjustScrollView();
    
    void muteTheMusic();
    void BeginGame();
    void backToMainScene();

    CREATE_FUNC(LevelSelectionScene);
    
    void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);
    virtual bool ccTouchBegan(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
};
