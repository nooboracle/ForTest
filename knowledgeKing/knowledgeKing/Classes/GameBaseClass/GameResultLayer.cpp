#include "GameBaseClass.h"
#include "ASGame.h"
#include "ASBot.h"
#include "ASUser.h"

extern ASUser* MainUser;
extern ASGame* player1;
extern ASBot* bot2;

void GameBaseClass::GameResult(int res){
    
    userSuccess = res;     //记录结果
    
    //1.战斗结果提示
    CCLayerColor* black = CCLayerColor::create(ccc4(0,0,0,130));
    black->setPosition(CCPointZero);
    addChild(black,1000,0);
    
    string resultStr;
    if (res == 0)
        resultStr = "Victory.png";
    else if(res == 1)
        resultStr = "Failed.png";
    
    CCSprite* result = CCSprite::createWithSpriteFrameName(resultStr.c_str());
    result->setScale(30);
    result->setPosition(ccp(size.width/2,winSize.height*11/20));
    black->addChild(result,1);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.5, 1.3);
    result->runAction(scaleDown);
    
    //2.触屏提示
    flashMessageBox("点击屏幕任意位置继续",CCPoint(size.width/2,winSize.height/3),ccc3(255,255,255));
}

void GameBaseClass::onEnter(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}
void GameBaseClass::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
bool GameBaseClass::ccTouchBegan(CCTouch* ptouch,CCEvent* event){
    return true;
}
void GameBaseClass::ccTouchMoved(CCTouch* ptouch, CCEvent* event){}
void GameBaseClass::ccTouchEnded(CCTouch* ptouch, CCEvent* pevent){
    
    //1.游戏结束以后
    if (gameover && !isIntroducing) {
        updateUserWinHTTP();
    }
    //2.进行教学中
    if (isIntroducing && screenCanBeTouched && !gameover){
        nextStep();
    }
    //3.教学结束
    if (isIntroducing && gameover) {
        nextStep();
    }
}