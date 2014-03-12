#include "ASGame.h"
#include "ASBot.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace CocosDenshion;

string imageToRemove[1000] = {};
int imageToRemoveCounter = 0;
extern ASBot* bot2;
extern string tibaoName[100];
extern ASUser* MainUser;

void ASGame::questionDraw(){
    
    int index = questionRandomedOrder[gameRound];
    cout<<index<<endl;
    
    //1.1 type: image question
    if (questionType[index] == 0 || questionType[index] == 3) {
        
        //1.1.1 image
        CCSprite* questionImage = CCSprite::create(questionImageFileName[index].c_str());
        questionImage->setPosition(ccp(size.width/2,size.height*15/24+winDif*2));
        addChild(questionImage,2,playerNumber*10000000+1);
        
        //1.1.2 if image size is small
        if (questionImage->getContentSize().height < 100)
            questionImage->setScale(1.7); 
        else if(questionImage->getContentSize().height >= 100 && questionImage->getContentSize().height < 300)
            questionImage->setScale(0.8);
        else if(questionImage->getContentSize().height >= 300 && questionImage->getContentSize().height < 400)
            questionImage->setScale(0.5);
        else if(questionImage->getContentSize().height >= 400)
            questionImage->setScale(0.3);
        
        //1.1.3 label
        CCLabelTTF* question = CCLabelTTF::create(questionLabel[index].c_str(),"Arial Rounded MT Bold",33.3,CCSizeMake(size.width*2/3,0),kCCTextAlignmentCenter);
        question->setColor(ccc3(100,100,100));
        question->setPosition(ccp(size.width/2, size.height*11.5/24+winDif*2));
        addChild(question,2,playerNumber*10000000+2);
        
        //1.1.4 记录游戏结束以后要从内存中清除的图片
        imageToRemove[imageToRemoveCounter] = questionImageFileName[index];
        imageToRemoveCounter++;
    }
    
    //2.2 label question
    else if(questionType[index] == 2) {
        CCLabelTTF* question = CCLabelTTF::create(questionLabel[index].c_str(),"Arial Rounded MT Bold",33.3,CCSizeMake(size.width*2/3,0),kCCTextAlignmentCenter);
        //CCLabelTTF* question = CCLabelTTF::create(test[0][index].c_str(),"Arial Rounded MT Bold",33.3,CCSizeMake(size.width*2/3,0),kCCTextAlignmentCenter);
        question->setColor(ccc3(100,100,100));
        question->setPosition(ccp(size.width/2, size.height*18.6/30+winDif*2));
        addChild(question,2,playerNumber*10000000+2);
    }
    
    //2.3 question source
    string source;
    if (questionSource[index]!=-1)
        source = tibaoName[questionSource[index]];
    else
        source = "天天答人大教学";
    CCLabelTTF* qSource = CCLabelTTF::create(source.c_str(), "Arial Rounded MT Bold", 17,CCSizeMake(size.width/5, 0),kCCTextAlignmentLeft);
    qSource->setPosition(ccp(size.width*13/50, size.height*8.5/20+winDif*2));
    qSource->setColor(ccc3(150,150,150));
    addChild(qSource,2,1*10000000+13);
}

void ASGame::rightOrWrongButtonDraw(){
    
    option1 = CCLabelTTF::create(twoAnswer[questionRandomedOrder[gameRound]][0].c_str(),"Arial Rounded MT Bold",25,CCSizeMake(size.width/6,0),kCCTextAlignmentCenter);
    option1->setColor(ccc3(60,60,60));
    option1->setPosition(ccp(size.width*27/96,size.height*71/256+winDif*2));
    addChild(option1,100,playerNumber*10000000+5);
    
    option2 = CCLabelTTF::create(twoAnswer[questionRandomedOrder[gameRound]][1].c_str(),"Arial Rounded MT Bold",25,CCSizeMake(size.width/6,0),kCCTextAlignmentCenter);
    option2->setColor(ccc3(60,60,60));
    option2->setPosition(ccp(size.width*68/96,size.height*71/256+winDif*2));
    addChild(option2,100,playerNumber*10000000+6);
}

void ASGame::userSelected(CCObject* sender){
    
    //1. 点击的是哪个
    int tag = ((CCMenuItemFont*)sender)->getTag() - (playerNumber*10000000+3);
    
    //2. 如果答对,则combo＋＋;
    if (tag != answer[questionRandomedOrder[gameRound]]){
        
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("right.mp3",false);
        //2.0 改变数据统计
        rightNumber++;
        
        //2.1 计算combo,记录Max Combo
        combo++;
        if (combo > MaxCombo) {MaxCombo = combo;}
        
        //2.2 right提示
        CCSprite* right = CCSprite::createWithSpriteFrameName("right.png");
        right->setOpacity(0);
        right->setPosition(ccp(size.width*5/40,size.height*81.5/90+winDif*2*alpha*alpha));
        addChild(right,2);
        CCActionInterval* fadeIn = CCFadeIn::create(0.5);
        CCActionInterval* JumpUp = CCJumpBy::create(0.7,ccp(0,0),20,1);
        CCActionInterval* effect = CCSpawn::create(fadeIn,JumpUp,NULL);
        CCActionInterval* fadeOut = CCFadeOut::create(0.3);
        CCCallFunc* remove = CCCallFuncN::create(this,callfuncN_selector(ASGame::removeSprite));
        CCSequence* seq = CCSequence::create(effect,fadeOut,remove,NULL);
        right->runAction(seq);
        
        //2.2 改变行动力
        int movePlus[20] = {1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,5,5};
        int moveToPlus;
        if (combo <= 19)
            moveToPlus = movePlus[combo-1];
        else
            moveToPlus = 5;
        moveCount += moveToPlus;
        changeMoveCount();
        changeEnergyBarAnimation(movePlus[combo-1]*100/moveToFight);
        
        //2.3 如果有hot
        if (hotRound > 0){
            hotRound--;
            hotAnimation();
            if (hotRound == 0) {
                removeChildByTag(1*10000000+103);
            }
        }
        
        //2.4 如果有dot
        if (dotRound > 0){
            dotRound--;
            if (dotRound == 0){
                removeChildByTag(1*10000000+102);
                bot2->stopDotAnimation();
            }
        }
    }
    
    //3.如果答错
    else{
        
        if(!MainUser->muted)
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("wrong.wav",false);
        
        //3.0 改变数据统计
        wrongNumber++;
        
        //3.1 wrong提示
        CCSprite* wrong = CCSprite::createWithSpriteFrameName("wrong.png");
        wrong->setOpacity(0);
        wrong->setPosition(ccp(size.width*5/40,size.height*81.5/90+winDif*2*alpha*alpha));
        addChild(wrong,2);
        CCActionInterval* fadeIn = CCFadeIn::create(0.5);
        CCActionInterval* JumpUp = CCJumpBy::create(0.7,ccp(0,0),20,2);
        CCActionInterval* effect = CCSpawn::create(fadeIn,JumpUp,NULL);
        CCActionInterval* fadeOut = CCFadeOut::create(0.3);
        CCCallFunc* remove = CCCallFuncN::create(this,callfuncN_selector(ASGame::removeSprite));
        CCSequence* seq = CCSequence::create(effect,fadeOut,remove,NULL);
        wrong->runAction(seq);
        
        //3.2 减血
        changeHeart(-maxShengMing*0.2,2);
        
        //3.3 如果不在super模式里的话
        combo = 0;
        removeChildByTag(playerNumber*10000000+29); //移除可以点击按钮的指示
        
        //3.4 改变行动力
        if (moveCount > 0 && shengming > 0) {
            CCMenu* energyContainerMenu = (CCMenu*)this->getChildByTag(1*10000000+27);
            energyContainerMenu->setTouchEnabled(false);
            moveCount--;
            changeMoveCount();
            changeEnergyBarAnimation(-100/moveToFight);
        }
        
        //3.5 如果有hot
        if (hotRound > 0){
            hotRound--;
            if (hotRound == 0)
                removeChildByTag(1*10000000+103);
        }
        
        //3.6 如果有dot
        if(dotRound > 0){
            dotRound--;
            changeHeart(-1000-33*(Level-1),2);
            if (dotRound == 0)
                removeChildByTag(1*10000000+102);
        }
    }
    
    if (shengming > 0) {
        unschedule(schedule_selector(ASGame::timeIsUp));
        goToNextRound();
    }
}

void ASGame::removeSprite(CCObject* sender){    removeChild((CCNode*)sender,true);      }
