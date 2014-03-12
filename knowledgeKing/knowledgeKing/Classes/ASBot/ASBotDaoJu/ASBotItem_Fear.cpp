#include "ASBot.h"
#include "ASGame.h"
#include <sys/timeb.h>

extern ASGame* player1;
/*
 不够完善，机制设计不佳，下一版考虑
void ASBot::FearButton(){
    //1.底座
    CCLayerColor* FearLayer = CCLayerColor::create(ccc4(0,0,0,100));
    FearLayer->setPosition(CCPointZero);
    player1->addChild(FearLayer,98,101);
    //2.禁用按钮
    ((CCMenu*)(player1->getChildByTag(1*10000000+12)))->setTouchEnabled(false);
    //3.开始到处跑
    schedule(schedule_selector(ASBot::ButtonBeginToRun),0.8);
}

void ASBot::ButtonBeginToRun(){
    
    unschedule(schedule_selector(ASBot::ButtonBeginToRun));
    
    struct timeb tp;
    ftime(&tp);
    srand(tp.millitm);
    
    int x1,x2,y1,y2;
    
    x1 = rand()%7+1;
    y1 = rand()%10+1;
    CCActionInterval* moveTo1 = CCMoveTo::create(0.4, ccp(size.width*x1/8,size.height*y1/11));
    player1->button1->runAction(moveTo1);
    CCActionInterval* moveTo2 = CCMoveTo::create(0.4, ccp(size.width*x1/8,size.height*y1/11));
    player1->option1->runAction(moveTo2);
    
    do{
       x2 = rand()%7+1;
       y2 = rand()%10+1;
    }while(x2 == x1 || y2 == y1);
    CCActionInterval* moveTo3 = CCMoveTo::create(0.4, ccp(size.width*x2/8,size.height*y2/11));
    player1->button2->runAction(moveTo3);
    CCActionInterval* moveTo4 = CCMoveTo::create(0.4, ccp(size.width*x2/8,size.height*y2/11));
    player1->option2->runAction(moveTo4);
    
    
    schedule(schedule_selector(ASBot::ButtonBeginToRun),1);
}

void ASBot::RandomRunAway(){
    
    struct timeb tp;
    ftime(&tp);
    srand(tp.millitm);
    
    //70%概率逃走
    int index = rand()%100+1;
    if (index <= 70)
        ButtonBeginToRun();
    else{
        ((CCMenu*)(player1->getChildByTag(1*10000000+12)))->setTouchEnabled(true);
        player1->removeChildByTag(101);
    }
}
*/