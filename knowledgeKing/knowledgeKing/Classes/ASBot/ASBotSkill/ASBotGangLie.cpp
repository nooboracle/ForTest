#include "ASBot.h"
#include "ASGame.h"
#include "global.h"

extern ASGame* player1;

void ASBot::botisGangLied(){
    
    CCSprite* ani = CCSprite::createWithSpriteFrameName("Miku_GL_0.png");
    ani->setRotation(-90);
    ani->setScale(2);
    ani->setPosition(ccp(size.width*5/40,size.height*80.3/90+winDif*2*alpha*alpha));
    player1->addChild(ani,100);
    
    CCAnimation* pAnimation = CCAnimation::create();
    for (int i = 0 ; i < 16; i++) {
        string texName = "Miku_GL_" + int2string(i) + ".png";
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str());
        pAnimation->addSpriteFrame(frame);
    }
    pAnimation->setDelayPerUnit(0.1);
    pAnimation->setRestoreOriginalFrame(true);
    pAnimation->setLoops(1);
    
    CCAnimate* pAnimate = CCAnimate::create(pAnimation);
    CCActionInterval* attack = CCMoveBy::create(1.8, ccp(size.width*30/40,0));
    CCActionInterval* effect = CCSpawn::create(pAnimate,attack,NULL);
    CCCallFunc* remove = CCCallFuncN::create(player1, callfuncN_selector(ASGame::removeSprite));
    CCCallFunc* add = CCCallFuncN::create(this, callfuncN_selector(ASBot::botMinusHpByGangLie));
    CCSequence* seq = CCSequence::create(effect,remove,add,NULL);
    
    ani->runAction(seq);
}

void ASBot::botMinusHpByGangLie(){
    
    int damageBack = (float)damageThisRound*(0.2+0.015*(Level-10));
    changeHeart(-damageBack,2);
}