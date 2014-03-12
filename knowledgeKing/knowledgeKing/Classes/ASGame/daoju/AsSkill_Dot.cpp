#include "ASGame.h"
#include "global.h"
#include "ASBot.h"

extern ASBot* bot2;

void ASGame::dotAnimation(){
    CCSprite* dotAnimation = CCSprite::createWithSpriteFrameName("LichKing_DOT_0.png");
    dotAnimation->setPosition(ccp(size.width*8.6/9.7,size.height/11+winDif*2*alpha*alpha));
    bot2->addChild(dotAnimation,100,2*10000000+102);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 28; i++) {
        string texName = "LichKing_DOT_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(-1);
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);

    dotAnimation->runAction(pAnimate);
}