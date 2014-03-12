#include "ASGame.h"
#include "ASBot.h"
#include "global.h"

extern ASBot* bot2;

void ASGame::silence(){
    
    if (bot2->silenceTime <= 0) {
        //1.敌人头像上的沉默动画
        CCSprite* ani = CCSprite::createWithSpriteFrameName("Sheldon_S_0.png");
        ani->setScale(1.3);
        ani->setOpacity(200);
        ani->setPosition(ccp(size.width*8.7/9.7,size.height*9.5/106+winDif*2*alpha*alpha));
        bot2->addChild(ani,4,2*10000000+9);
        
        CCAnimation* pAnimation = CCAnimation::create();
        for (int i = 0 ; i < 29; i++) {
            string texName = "Sheldon_S_" + int2string(i) + ".png";
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
            pAnimation->addSpriteFrame(frame);
        }
        pAnimation->setDelayPerUnit(0.1);
        pAnimation->setRestoreOriginalFrame(true);
        pAnimation->setLoops(-1);
        CCAnimate* pAnimate = CCAnimate::create(pAnimation);
        ani->runAction(pAnimate);
        
        //2.禁用对方道具
        bot2->canUseItem = false;
        
        //3.计算时间
        bot2->silenceTime = 20 + 0.5*(Level-1);
        schedule(schedule_selector(ASGame::countSilenceTime),1);
        
        //4.时间提示
        CCLabelTTF* silenceTimeLabel = CCLabelTTF::create(int2string(bot2->silenceTime).c_str(), "Arial Rounded MT bold",35);
        silenceTimeLabel->setPosition(ccp(size.width*9/10.2,size.height*9/99+winDif*2*alpha*alpha));
        bot2->addChild(silenceTimeLabel,5,2*10000000+8);
    }
}

void ASGame::countSilenceTime(){
    
    bot2->silenceTime--;
    CCLabelTTF* silenceTimeLabel = (CCLabelTTF*)bot2->getChildByTag(2*10000000+8);
    silenceTimeLabel->setString(int2string(bot2->silenceTime).c_str());
    
    if (bot2->silenceTime <= 0) {
        
        unschedule(schedule_selector(ASGame::countSilenceTime));
        
        bot2->removeChildByTag(2*10000000+8);
        bot2->removeChildByTag(2*10000000+9);
        
        bot2->canUseItem = true;
    }
}

