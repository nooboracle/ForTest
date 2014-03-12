#ifndef __knowledgeKing__TuiTuMapScene__
#define __knowledgeKing__TuiTuMapScene__

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace extension;

class TuiTuMapScene : public CCLayer, public CCScrollViewDelegate{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    CCScrollView* ScrollView;
    CCLayerColor* TiBaoLayer;
    
    void basicMapDraw();
    void chapterIsSelected(CCNode* sender);
    void TiBaoListDraw();
    void SuccessRequirementDraw();
    void openTiBaoLayer();
    void removeTiBaoLayer(CCNode* sender);
    
    
    
    CREATE_FUNC(TuiTuMapScene);
    
    void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);
    virtual bool ccTouchBegan(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
};


#endif /* defined(__knowledgeKing__TuiTuMapScene__) */
