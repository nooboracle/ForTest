#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class VideoScene : public cocos2d::CCLayer{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    void goGameBridge();
    
    CREATE_FUNC(VideoScene);
};