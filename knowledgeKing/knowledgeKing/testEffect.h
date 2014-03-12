#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class testEffect : public cocos2d::CCLayer{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    void Repeat(CCNode* sender);
    
    CREATE_FUNC(testEffect);
};