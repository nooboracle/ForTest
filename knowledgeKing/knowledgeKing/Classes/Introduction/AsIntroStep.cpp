#include "GameBaseClass.h"
#include "ASBotFightLayer.h"
#include "ASGame.h"
#include "ASBot.h"

extern ASGame* player1;
extern ASBot* bot2;

void GameBaseClass::nextStep(){
    stepCounter++;
    if (stepCounter == 2)
        IntroStep2();
    else if(stepCounter == 3)
        IntroStep3();
    else if(stepCounter == 4)
        IntroStep4();
    else if(stepCounter == 5)
        IntroStep5();
    else if(stepCounter == 6)
        IntroStep6();
    else if(stepCounter == 7)
        IntroStep7();
    else if(stepCounter == 8)
        IntroStep8();
    else if(stepCounter == 9)
        IntroStep9();
    else if(stepCounter == 10)
        IntroStep10();
    else if(stepCounter == 11)
        IntroStep11();
    else if(stepCounter == 12)
        IntroStep12();
    else if(stepCounter == 13)
        IntroStep13();
    else if(stepCounter == 14)
        IntroStep14();
    else if(stepCounter == 15)
        IntroStep15();
    else if(stepCounter == 16)
        IntroStep16();
}

void GameBaseClass::IntroStep1(){
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("2.Introduction.plist");
    
    //1.贴图
    CCSprite* step1_1 = CCSprite::createWithSpriteFrameName("step1_1.png");
    CCSprite* step1_2 = CCSprite::createWithSpriteFrameName("step1_2.png");
    CCSprite* step1_3 = CCSprite::createWithSpriteFrameName("step1_3.png");
    CCSprite* step1_4 = CCSprite::createWithSpriteFrameName("step1_4.png");
    CCSprite* step1_5 = CCSprite::createWithSpriteFrameName("step1_5.png");
    CCSprite* step1_6 = CCSprite::createWithSpriteFrameName("step1_6.png");
    step1_1->setPosition(ccp(winSize.width/2,size.height*142.5/2048));
    step1_2->setPosition(ccp(winSize.width*1368/1536,size.height*170.5/2048));
    step1_3->setPosition(ccp(winSize.width*168/1536,size.height*170.5/2048));
    step1_4->setPosition(ccp(winSize.width*1174.5/1536,size.height*1865.5/2048+winDif*2*alpha*alpha));
    step1_5->setPosition(ccp(winSize.width*361.5/1536,size.height*1865.5/2048+winDif*2*alpha*alpha));
    step1_6->setPosition(ccp(winSize.width/2,size.height*9.8/20+winDif*1.6));
    introductionLayer->addChild(step1_1,1,2);
    introductionLayer->addChild(step1_2,1,3);
    introductionLayer->addChild(step1_3,1,4);
    introductionLayer->addChild(step1_4,1,5);
    introductionLayer->addChild(step1_5,1,6);
    introductionLayer->addChild(step1_6,1,7);
    
    //2.禁用所有按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    skillMenu->setTouchEnabled(false);
    optionMenu->setTouchEnabled(false);
    itemMenu->setTouchEnabled(false);
    
    //3.闪屏提示
    flashMessageBox("点击屏幕任意位置继续",CCPoint(size.width/2,size.height*14.4/30+winDif*alpha*alpha),ccc3(255,255,255));
}

void GameBaseClass::IntroStep2(){
    
    //1.移除上一步提示
    introductionLayer->removeChildByTag(2);
    introductionLayer->removeChildByTag(3);
    introductionLayer->removeChildByTag(4);
    introductionLayer->removeChildByTag(5);
    introductionLayer->removeChildByTag(6);
    introductionLayer->removeChildByTag(7);
    removeChild(touchToGoOn);

    //2.下一步
    CCSprite* step2 = CCSprite::createWithSpriteFrameName("step2.png");
    step2->setPosition(ccp(size.width*10.7/20,size.height*14.7/20+winDif*2*alpha*alpha));
    introductionLayer->addChild(step2,1,2);
    
    //3.闪屏提示
    flashMessageBox("点击屏幕任意位置继续",CCPoint(size.width/2,size.height*14.4/30+winDif*alpha*alpha),ccc3(55,55,55));
}

void GameBaseClass::IntroStep3(){
    
    //0.关闭这个状态，强制用户点击正确的按钮
    screenCanBeTouched = false;
    
    //1.移除上一步
    introductionLayer->removeChildByTag(2);
    removeChild(touchToGoOn);
    
    //2.下一步
    CCSprite* step3 = CCSprite::createWithSpriteFrameName("step3.png");
    step3->setPosition(ccp(size.width*5.5/20,size.height*8.5/20+winDif*1.6));
    introductionLayer->addChild(step3,1,2);
    
    //3.启用左侧按钮
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    optionMenu->setTouchEnabled(true);
    CCMenuItemSprite* rightSideButton = (CCMenuItemSprite*)optionMenu->getChildByTag(1*10000000+4);
    rightSideButton->setEnabled(false);
}

void GameBaseClass::IntroStep4(){
    
    //1.移除上一步提示
    introductionLayer->removeChildByTag(2);
    
    //2.下一步
    CCSprite* step4 = CCSprite::createWithSpriteFrameName("step4.png");
    step4->setPosition(ccp(size.width*666.5/1536,size.height*185.5/2048));
    introductionLayer->addChild(step4,1,2);
    
    //3.禁用左侧按钮
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    optionMenu->setTouchEnabled(false);
    CCMenuItemSprite* rightSideButton = (CCMenuItemSprite*)optionMenu->getChildByTag(1*10000000+3);
    rightSideButton->setEnabled(false);
    
    //4.启用技能按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    skillMenu->setTouchEnabled(true);
}

void GameBaseClass::IntroStep5(){
    
    //0.打开这个状态
    screenCanBeTouched = true;
    
    //2.移除上一步提示
    introductionLayer->removeChildByTag(2);
    
    //3.下一步
    CCSprite* step5 = CCSprite::createWithSpriteFrameName("step5.png");
    step5->setPosition(ccp(size.width*7.5/20,size.height*17/20+winDif*2*alpha*alpha));
    introductionLayer->addChild(step5,1,2);
    
    //4.禁用所有按钮,因为战斗画面会重置这些按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    skillMenu->setTouchEnabled(false);
    optionMenu->setTouchEnabled(false);
    itemMenu->setTouchEnabled(false);
    
    flashMessageBox("点击屏幕任意位置继续",CCPoint(size.width/2,size.height*14.4/30+winDif*alpha*alpha),ccc3(51, 51, 51));
}

void GameBaseClass::IntroStep6(){
    
    //1.屏幕不能点击
    screenCanBeTouched = false;
    
    //2.移除上一步提示
    introductionLayer->removeChildByTag(2);
    removeChild(touchToGoOn);
    
    //3.下一步
    CCSprite* step6 = CCSprite::createWithSpriteFrameName("step6.png");
    step6->setPosition(ccp(size.width*12.5/20,size.height*15/20+winDif*2*alpha*alpha));
    introductionLayer->addChild(step6,1,2);
    
    //4.乌云开始计算时间
    player1->schedule(schedule_selector(ASGame::countCloudTime),1);
    
    //5.对手开始答题，连错三道
    bot2->botAnswer = 1;
    schedule(schedule_selector(GameBaseClass::botAnswerQuestion),2);
    
    //6.禁用所有按钮,因为战斗画面会重置这些按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    skillMenu->setTouchEnabled(false);
    optionMenu->setTouchEnabled(false);
    itemMenu->setTouchEnabled(false);
    
    flashMessageBox("对手正在被乌云干扰，请围观",CCPoint(size.width/2,size.height*14.4/30+winDif*alpha*alpha),ccc3(55, 55, 55));
    
}

void GameBaseClass::botAnswerQuestion(){
    if (botAnswerCounter == 3){
        bot2->botAnswer = 0;
        bot2->botSelected();
        botAnswerCounter++;
    }
    else if(botAnswerCounter == 4){
        unschedule(schedule_selector(GameBaseClass::botAnswerQuestion));
        nextStep();
    }
    else{
        bot2->botSelected();
        botAnswerCounter++;
    }    
}

void GameBaseClass::IntroStep7(){
    
    //0.打开这个状态
    screenCanBeTouched = true;
    
    //2.移除上一步提示
    introductionLayer->removeChildByTag(2);
    removeChild(touchToGoOn);
    
    //3.下一步
    CCSprite* step7 = CCSprite::createWithSpriteFrameName("step7.png");
    step7->setPosition(ccp(size.width*13/20,size.height*15/20+winDif*2*alpha*alpha));
    introductionLayer->addChild(step7,1,2);
    
    //4.禁用所有按钮,因为战斗画面会重置这些按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    skillMenu->setTouchEnabled(false);
    optionMenu->setTouchEnabled(false);
    itemMenu->setTouchEnabled(false);
    
    flashMessageBox("点击屏幕任意位置继续",CCPoint(size.width/2,size.height*14.4/30+winDif*alpha*alpha),ccc3(55, 55, 55));
}

void GameBaseClass::IntroStep8(){
    
    //1.关闭这个状态
    screenCanBeTouched = false;
    
    //2.移除上一步提示
    introductionLayer->removeChildByTag(2);
    removeChild(touchToGoOn);
    
    //3.发动攻击
    bot2->damageThisRound = bot2->calDamage();
    ASBotFightLayer* attack = ASBotFightLayer::createAsFightLayer(1,1,false,bot2->damageThisRound,2,player1->moveCount,player1->shengming,bot2->moveCount,bot2->shengming);
    attack->setPosition(CCPointZero);
    addChild(attack,9999,9999);
    
    //4.禁用所有按钮,因为战斗画面会重置这些按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    skillMenu->setTouchEnabled(false);
    optionMenu->setTouchEnabled(false);
    itemMenu->setTouchEnabled(false);
}

void GameBaseClass::IntroStep9(){
    
    //0.打开这个状态
    screenCanBeTouched = true;
    
    //1.下一步
    CCSprite* step8 = CCSprite::createWithSpriteFrameName("step8.png");
    step8->setPosition(ccp(size.width/2,size.height*8/20+winDif*1.6));
    introductionLayer->addChild(step8,1,2);
    
    //2.禁用所有按钮,因为战斗画面会重置这些按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    skillMenu->setTouchEnabled(false);
    optionMenu->setTouchEnabled(false);
    itemMenu->setTouchEnabled(false);
    
    flashMessageBox("点击屏幕任意位置继续",CCPoint(size.width/2,size.height*14.4/30+winDif*alpha*alpha),ccc3(55, 55, 55));
}

void GameBaseClass::IntroStep10(){
    
    //0.关闭这个状态
    screenCanBeTouched = false;
    
    //2.移除上一步提示
    introductionLayer->removeChildByTag(2);
    removeChild(touchToGoOn);
    
    //3.下一步
    CCSprite* step9 = CCSprite::createWithSpriteFrameName("step9.png");
    step9->setPosition(ccp(size.width*11.4/20,size.height*4.5/20));
    introductionLayer->addChild(step9,1,2);
    
    //3.禁用技能按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    skillMenu->setTouchEnabled(false);
    
    //4.启用道具按钮
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    optionMenu->setTouchEnabled(true);
    CCMenuItemSprite* first = (CCMenuItemSprite*)optionMenu->getChildByTag(1*10000000+77777);
    first->setEnabled(false);
    CCMenuItemSprite* third = (CCMenuItemSprite*)optionMenu->getChildByTag(1*10000000+77779);
    third->setEnabled(false);
    
    
}

void GameBaseClass::IntroStep11(){
    
    //0.打开这个状态
    screenCanBeTouched = true;
    
    //1.移除上一步提示
    introductionLayer->removeChildByTag(2);
    
    //2.下一步
    CCSprite* step10 = CCSprite::createWithSpriteFrameName("step10.png");
    step10->setPosition(ccp(size.width*9.6/20,size.height*14.6/20+winDif*2*alpha*alpha));
    introductionLayer->addChild(step10,1,2);
    
    //3.禁用技能按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    skillMenu->setTouchEnabled(false);
    
    //3.禁用道具按钮
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    optionMenu->setTouchEnabled(false);
    CCMenuItemSprite* first = (CCMenuItemSprite*)optionMenu->getChildByTag(1*10000000+77778);
    first->setEnabled(false);
    
    //4.添加用户的行动力
    player1->moveCount += 25;
    player1->changeMoveCount();
    player1->changeEnergyBarAnimation(25);
    
    flashMessageBox("点击屏幕任意位置继续",CCPoint(size.width/2,size.height*14.4/30+winDif*alpha*alpha),ccc3(55, 55, 55));
}

void GameBaseClass::IntroStep12(){
    
    //0.关闭这个状态
    screenCanBeTouched = false;
    
    //1.移除上一步提示
    introductionLayer->removeChildByTag(2);
    removeChild(touchToGoOn);
    
    //2.下一步
    CCSprite* step11 = CCSprite::createWithSpriteFrameName("step3.png");
    step11->setPosition(ccp(size.width*5.5/20,size.height*8.5/20+winDif*1.6));
    introductionLayer->addChild(step11,1,2);
    
    //3.禁用技能按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    skillMenu->setTouchEnabled(false);
    
    //3.启用左侧按钮
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    optionMenu->setTouchEnabled(true);
    CCMenuItemSprite* rightSideButton = (CCMenuItemSprite*)optionMenu->getChildByTag(1*10000000+3);
    rightSideButton->setEnabled(true);
}

void GameBaseClass::IntroStep13(){
    
    //3.禁用技能按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    skillMenu->setTouchEnabled(false);
    
    //空，因为也是答一道题
}

void GameBaseClass::IntroStep14(){
    
    //0.关闭这个状态
    screenCanBeTouched = false;
    
    //1.移除上一步提示
    introductionLayer->removeChildByTag(2);
    
    //2.下一步
    CCSprite* step11 = CCSprite::createWithSpriteFrameName("step11.png");
    step11->setPosition(ccp(size.width*9.85/20,size.height*14.5/20+winDif*2*alpha*alpha));
    introductionLayer->addChild(step11,1,2);
    
    //3.关闭左侧按钮
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    optionMenu->setTouchEnabled(false);
    CCMenuItemSprite* rightSideButton = (CCMenuItemSprite*)optionMenu->getChildByTag(1*10000000+3);
    rightSideButton->setEnabled(false);
    
    //4.启用技能按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    skillMenu->setTouchEnabled(true);
}

void GameBaseClass::IntroStep15(){
    
    //0.可以点击
    screenCanBeTouched = true;
    
    //2.移除上一步提示
    introductionLayer->removeChildByTag(2);
    
    //3.禁用所有按钮,因为战斗画面会重置这些按钮
    CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
    CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
    CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
    skillMenu->setTouchEnabled(false);
    optionMenu->setTouchEnabled(false);
    itemMenu->setTouchEnabled(false);
}

void GameBaseClass::IntroStep16(){
    
    //0.记录胜利
    userSuccess = 0;
    screenCanBeTouched = true;
    
    //1.移除上一步提示
    introductionLayer->removeChildByTag(2);
    removeChild(touchToGoOn,true);
    
    //2.技术统计页面
    CongScene();
}

void GameBaseClass::flashMessageBox(string _message,CCPoint _pos,ccColor3B _color){
    touchToGoOn = CCLabelTTF::create(_message.c_str(),"Arial Rounded MT bold",32);
    touchToGoOn->setColor(_color);
    touchToGoOn->setPosition(_pos);
    addChild(touchToGoOn,10001);
    
    CCActionInterval* fadeIn = CCFadeIn::create(0.5);
    CCActionInterval* fadeOut = CCFadeOut::create(0.5);
    CCSequence* flashEffect = CCSequence::create(fadeIn,fadeOut,NULL);
    CCRepeatForever* repeatFlash = CCRepeatForever::create(flashEffect);
    touchToGoOn->runAction(repeatFlash);
}
