#include "ASBot.h"
#include "ASGame.h"
#include "global.h"

extern ASGame* player1;

void ASBot::botSilence(){
    if (player1->silenceTime <= 0) {
        
        //2.用户头像上的沉默动画
        CCSprite* ani = CCSprite::createWithSpriteFrameName("Sheldon_S_0.png");
        ani->setScale(1.4);
        ani->setPosition(ccp(size.width*1.08/9.7,size.height*10.3/11+winDif*2*alpha*alpha));
        player1->addChild(ani,4,1*10000000+9);
        
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
        
        //3.禁用
        CCMenu* menu =  (CCMenu*)player1->getChildByTag(1*10000000+77776);
        menu->setTouchEnabled(false);
        
        //4.计算时间
        player1->silenceTime = 20 + 0.5*(Level-1);
        schedule(schedule_selector(ASBot::countSilenceTime),1);
        
        //5.时间提示
        CCLabelTTF* silenceTimeLabel = CCLabelTTF::create(int2string(player1->silenceTime).c_str(), "Arial Rounded MT bold",40);
        silenceTimeLabel->setPosition(ccp(size.width*1.10/9.7,size.height*10/11+winDif*2*alpha*alpha));
        player1->addChild(silenceTimeLabel,5,1*10000000+8);
        
        //6.道具上的红色禁用标识
        CCPoint pos[3] = {ccp(size.width*510/1500,size.height*8.5/150),ccp(size.width*755/1500, size.height*8.5/150),ccp(size.width*1000/1500, size.height*8.5/150)};
        for (int i = 0 ; i < 3 ; i++) {
            CCSprite* cannotuseitemSprite = CCSprite::createWithSpriteFrameName("cannotUseItem.png");
            cannotuseitemSprite->setPosition(pos[i]);
            player1->addChild(cannotuseitemSprite,5,1*10000000+17+i);
        }
    }
}

void ASBot::countSilenceTime(){
    
    player1->silenceTime--;
    CCLabelTTF* silenceTimeLabel = (CCLabelTTF*)player1->getChildByTag(1*10000000+8);
    silenceTimeLabel->setString(int2string(player1->silenceTime).c_str());
    
    if (player1->silenceTime <= 0) {
        
        unschedule(schedule_selector(ASBot::countSilenceTime));
        
        player1->removeChildByTag(1*10000000+8);
        player1->removeChildByTag(1*10000000+9);
        player1->removeChildByTag(1*10000000+17);
        player1->removeChildByTag(1*10000000+18);
        player1->removeChildByTag(1*10000000+19);
        
        CCMenu* menu =  (CCMenu*)player1->getChildByTag(1*10000000+77776);
        menu->setTouchEnabled(true);
    }
}
