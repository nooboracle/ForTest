#include "ASGame.h"
#include "ASBot.h"

extern ASBot* bot2;

void ASGame::goThiefKing(){
    
    if (!bot2->ThiefFollowed) {
        
        bot2->ThiefFollowed = true;
        
        CCTexture2D* djImageTexture = CCTextureCache::sharedTextureCache()->addImage("daoju_0.png");
        CCSprite* botImage = (CCSprite*)bot2->getChildByTag(2*10000000+60);
        botImage->setTexture(djImageTexture);
    }
}

void ASGame::scoreIsStolen(int _score){
    
}