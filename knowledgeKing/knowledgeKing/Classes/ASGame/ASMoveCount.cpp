#include "ASGame.h"
#include "global.h"
#include "ASFightLayer.h"
#include "GameBaseClass.h"
#include "SimpleAudioEngine.h"
#include "ASBot.h"
#include "ASUser.h"
using namespace std;
using namespace CocosDenshion;

extern ASBot* bot2;
extern ASUser* MainUser;

void ASGame::changeMoveCount(){

    //1.改变数字
    string tmp = int2string(moveCount);
    CCLabelTTF* number = (CCLabelTTF*)this->getChildByTag(playerNumber*10000000+26);
    number->setScale(30);
    number->setString(tmp.c_str());
    
    CCActionInterval* ScaleDown = CCScaleTo::create(0.15,1);
    number->runAction(ScaleDown);
    
    //2.启用和禁用按钮
    CCMenu* energyContainerMenu = (CCMenu*)this->getChildByTag(1*10000000+27);
    CCProgressTimer* attackButton = (CCProgressTimer*)this->getChildByTag(1*10000000+25);
    if (moveCount >= moveToFight){
        
        //1.攻击按钮科技点击
        if(!((GameBaseClass*)getParent())->isIntroducing)
            energyContainerMenu->setTouchEnabled(true);
        
        //2.攻击按钮一直闪烁
        attackButton->setAnchorPoint(ccp(0.5,0.5));
        CCActionInterval* scaleUp = CCScaleTo::create(0.5, 1.2);
        CCActionInterval* scaleDown = CCScaleTo::create(0.5, 1);
        CCSequence* seq = CCSequence::create(scaleUp,scaleDown,NULL);
        CCRepeatForever* effect = CCRepeatForever::create(seq);
        attackButton->runAction(effect);
    }
    else{
        energyContainerMenu->setTouchEnabled(false);
        attackButton->stopAllActions();
        attackButton->setScale(1);
    }
}

void ASGame::enterFightLayer(){
    
    if (!((GameBaseClass*)getParent())->gameover) {
        
        if(!MainUser->muted){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
            string attackSound[6] = {"IronMan.mp3","miku.mp3","kuaipao.wav","bazinga.mp3","BM.wav"};
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(attackSound[rolerTag-1].c_str());
        }
        
        //1.禁用大招按钮
        CCMenu* skillMenu = (CCMenu*)this->getChildByTag(1*10000000+27);
        skillMenu->setTouchEnabled(false);
        
        //2.禁用答题按钮
        CCMenu* optionMenu = (CCMenu*)this->getChildByTag(1*10000000+12);
        optionMenu->setTouchEnabled(false);
        
        //3.禁用道具栏
        CCMenu* itemMenu = (CCMenu*)this->getChildByTag(playerNumber*10000000+77776);
        itemMenu->setTouchEnabled(false);
        
        //4.停止所有监听
        unscheduleAllSelectors();
        bot2->unscheduleAllSelectors();
        
        //5.播放战斗动画
        damageThisRound = calDamage();
        ASFightLayer* aaa = ASFightLayer::createAsFightLayer(rolerTag,bot2->rolerTag,isCritical,damageThisRound,1,moveCount,shengming,bot2->moveCount,bot2->shengming);
        aaa->setPosition(CCPointZero);
        getParent()->addChild(aaa,9999,9999);
    }
}

void ASGame::afterFightAnimation(){
        
    //1.无论如何对面都减血，清空行动力
    bot2->changeHeart(-damageThisRound,1);
    moveCount = 0;
    changeMoveCount();
    changeEnergyBarAnimation(-100);
    combo = 0;
    isCritical = false;
    
    //2.如果不是教学模式，并且这一刀对面也没有死的话，则:
    //(1)重新开始计时间（玩家答题时间，乌云，沉默，爆击药水)，机器人继续思考
    //(2)启用按钮（答题按钮，道具栏） 技能按钮因为movecount为0，所以不能使用
    //(3)各种技能附带的效果生效
    if (!((GameBaseClass*)getParent())->isIntroducing && !((GameBaseClass*)getParent())->gameover) {
        
    /***********计时器***********/
        //2.0 答题与机器人
        schedule(schedule_selector(ASGame::timeIsUp),0.04);
        bot2->botIsThinkingAboutAnswer();
        bot2->schedule(schedule_selector(ASBot::botReadyToEnterFightLayer), 5);
        bot2->botItemPlan();
        
        //2.1 乌云
        if (clouded)
            bot2->schedule(schedule_selector(ASBot::countCloudTime),1);
        if (bot2->clouded)
            schedule(schedule_selector(ASGame::countCloudTime),1);
        
        //2.2 沉默
        if (silenceTime > 0)
            bot2->schedule(schedule_selector(ASBot::countSilenceTime),1);
        if (bot2->silenceTime > 0)
            schedule(schedule_selector(ASGame::countSilenceTime),1);
        
        //2.3 爆击
        if (criticalAddedTime > 0)
            schedule(schedule_selector(ASGame::CriticalRateCounter));
        if (bot2->criticalAddedTime > 0)
            bot2->schedule(schedule_selector(ASBot::CriticalRateCounter));
     
     /***********启用按钮***********/
        
        //2.4 启用答题按钮
        CCMenu* optionMenu = (CCMenu*)this->getChildByTag(1*10000000+12);
        optionMenu->setTouchEnabled(true);
        
        //2.5 启用道具栏
        CCMenu* itemMenu = (CCMenu*)this->getChildByTag(playerNumber*10000000+77776);
        itemMenu->setTouchEnabled(true);
        
     /***********主动技能附加效果***********/
        
        //3.1 钢铁侠，附加乌云
        if(rolerTag == 1)
            coverQuestion(10+0.2*(Level-1));
        //3.2 初音，自身获得HOT
        else if(rolerTag == 2){//初音，hot
            hotRound = 2;
            hotBuff();
        }
        //3.3 巫妖王，向对面施加DOT
        else if(rolerTag == 3 && bot2->dotRound < 2){
            if (bot2->dotRound == 0)
                dotAnimation();
            bot2->dotRound = 2;
        }
        //3.4 谢耳朵，沉默对手
        else if(rolerTag == 4)
            silence();
        //3.5 剑圣,自己也减血，但不能致死,最后剩1滴血
        else if(rolerTag == 5){
            if (shengming > 600-10*(Level-1))
                changeHeart(-(600-10*(Level-1)),2);
            else
                changeHeart(-(shengming-1),2);
        }
        
        //3.6 路飞，进入决斗模式
        else if (rolerTag == 6){}
        
      /***********对手天赋技能附加效果，主要为反馈***********/
        
        //4.0 对手是初音，会被刚烈
        if (bot2->rolerTag == 2 && bot2->Level >= 10)
            userIsGangLied();
    }
    
    //3.如果是教学模式，则释放乌云，然后下一步   教学模式true，gameover 对或错
    else if(((GameBaseClass*)getParent())->isIntroducing){
        coverQuestion(10+0.2*(Level-1));
        ((GameBaseClass*)getParent())->nextStep();
    }
    
    /*
    //4.如果这一刀先砍死对面，则胜利   gameover对，不在教学模式里
    else if(((GameBaseClass*)getParent())->gameover && !((GameBaseClass*)getParent())->isIntroducing){
        unscheduleAllSelectors();
        bot2->unscheduleAllSelectors();
        ((GameBaseClass*)(this->getParent()))->GameResult(0);
    }
     */
}




