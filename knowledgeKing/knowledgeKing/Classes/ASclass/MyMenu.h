#ifndef __knowledgeKing__MyMenu__
#define __knowledgeKing__MyMenu__
#include "cocos2d.h"
using namespace cocos2d;

class MyMenu : public CCMenu
{
public:
    
    bool moved = false;
    static MyMenu* create();
    static MyMenu* create(CCMenuItem* item, ...);
    static MyMenu* createWithArray(CCArray* pArrayOfItems);
    static MyMenu* createWithItems(CCMenuItem* item, va_list args);
    
    
    virtual void registerWithTouchDispatcher();
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

};

#endif /* defined(__knowledgeKing__MyMenu__) */