#include "AsMessageBox.h"
#include <sys/timeb.h>
#include "global.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

void AsMessageBox::initLoadingBox(string _message){
    
    struct timeb tp;
    ftime(&tp);
    std::srand(tp.millitm);
    LoadingCounter = 0;
        
    //1.黑色底层
    cover = CCLayerColor::create(ccc4(0,0,0,200));
    cover->setPosition(CCPointZero);
    addChild(cover,0);
    
    //2.提示框底层
    showbox = CCSprite::createWithSpriteFrameName("LoadingFrame.png");
    showbox->setPosition(ccp(size.width/2,size.height/2+winDif*2*alpha));
    cover->addChild(showbox,1);
    
    //3.提示框上的文字
    message = CCLabelTTF::create(_message.c_str(), "Arial Rounded MT bold", 30);
    message->setPosition(ccp(size.width/2,size.height*9.3/20+winDif*2*alpha));
    cover->addChild(message,2);
    
    //2.提示框上的小脑袋
    int headIndex = rand()%6+1;
    string tex = "head_" + int2string(headIndex) + ".png";
    for (int i = 0;  i < 3 ; i ++) {
        littleHead[i] = CCSprite::createWithSpriteFrameName(tex.c_str());
        littleHead[i]->setOpacity(0);
        littleHead[i]->setPosition(ccp(size.width/2-size.width/14+size.width*i/15, size.height*10.5/20+winDif*2*alpha));
        cover->addChild(littleHead[i],3);
    }
    
    //5.提示框上的人物
    string heroTex[6] = {"BladeMaster.png","IronMan.png","Miku.png","Sheldon.png","LichKing.png","Luffy.png"};
    CCSprite* hero = CCSprite::createWithSpriteFrameName(heroTex[headIndex-1].c_str());
    hero->setScale(0.5);
    hero->setPosition(ccp(size.width*2/7, size.height*17/30+winDif*2*alpha));
    cover->addChild(hero,2);
    
    //6.小脑袋一直动
    schedule(schedule_selector(AsMessageBox::RollHead),1);
    
    //7.超时计时器
    schedule(schedule_selector(AsMessageBox::LoadingTimeCounter),1);
}

void AsMessageBox::RollHead(){
    
    if (headCounter < 3){
        littleHead[headCounter]->setOpacity(255);
        headCounter++;
    }
    else{
        headCounter = 0;
        littleHead[0]->setOpacity(0);
        littleHead[1]->setOpacity(0);
        littleHead[2]->setOpacity(0);
    }
}

void AsMessageBox::LoadingTimeCounter(){
    if (LoadingCounter < 180)
        LoadingCounter++;
    else{
        unschedule(schedule_selector(AsMessageBox::LoadingTimeCounter));
        closeMessageBox();
        CCMessageBox("网络超时,请检查您的网络", "温馨提示");
    }
}