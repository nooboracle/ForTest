#include "AsMessageBox.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

void AsMessageBox::initFightInfo(string _message){
    
    message = CCLabelTTF::create(_message.c_str(), "Arial", 50);
    message->setPosition(ccp(size.width/2, size.height*6/7+winDif*2*alpha));
    addChild(message,0);
    
    CCActionInterval* fadeIn = CCFadeIn::create(0.5);
    CCActionInterval* moveUp = CCMoveBy::create(0.5, ccp(0, size.height/14));
    CCActionInterval* fadeOut = CCFadeOut::create(0.5);
    
    CCActionInterval* effect = CCSpawn::create(fadeIn,moveUp,NULL);
    CCSequence* SEQ = CCSequence::create(effect,fadeOut,NULL);
    message->runAction(SEQ);
}

void AsMessageBox::addFightMessage(string _message, int colorIndex){
    
    CCLabelTTF* message1 = CCLabelTTF::create(_message.c_str(), "Arial Rounded MT bold", 50);
    message1->setPosition(ccp(size.width/2, size.height*6/7+winDif*2*alpha));
    addChild(message,0);
    
    CCActionInterval* fadeIn = CCFadeIn::create(0.5);
    CCActionInterval* moveUp = CCMoveBy::create(0.5, ccp(0, size.height/14));
    CCActionInterval* fadeOut = CCFadeOut::create(0.5);
    
    CCActionInterval* effect = CCSpawn::create(fadeIn,moveUp,NULL);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(AsMessageBox::removeThis));
    CCSequence* SEQ = CCSequence::create(effect,fadeOut,remove,NULL);
    message->runAction(SEQ);

    if (colorIndex == 1)
        message->setColor(ccc3(117, 224, 96));
    else if (colorIndex == 2)
        message->setColor(ccc3(242, 48, 41));
}


