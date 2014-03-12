#include "ASBot.h"
#include "global.h"
#include "ASGame.h"
#include "ASBotFightLayer.h"
#include "GameBaseClass.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
using namespace CocosDenshion;
using namespace std;

extern ASGame* player1;
extern ASBot* bot2;
extern ASUser* MainUser;

void ASBot::changeMoveCount(){
    
    string tmp = int2string(moveCount);
    CCLabelTTF* number = (CCLabelTTF*)this->getChildByTag(2*10000000+26);
    number->setScale(30);
    number->setString(tmp.c_str());
    
    CCActionInterval* ScaleDown = CCScaleTo::create(0.15,1);
    number->runAction(ScaleDown);
}

void ASBot::enterFightLayer(){
    
    if (!((GameBaseClass*)getParent())->gameover) {
        
        if(!MainUser->muted){
            CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
            string attackSound[6] = {"IronMan.mp3","miku.mp3","kuaipao.wav","bazinga.mp3","BM.wav"};
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(attackSound[rolerTag-1].c_str());
        }
        
        //1.停止所有监听
        unscheduleAllSelectors();
        player1->unscheduleAllSelectors();
        
        //2.禁用我方的技能按钮
        CCMenu* skillMenu = (CCMenu*)player1->getChildByTag(1*10000000+27);
        skillMenu->setTouchEnabled(false);
        
        //3.禁用答题按钮
        CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
        optionMenu->setTouchEnabled(false);
        
        //4.禁用道具按钮
        CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
        itemMenu->setTouchEnabled(false);
        
        //4.播放战斗动画
        damageThisRound = calDamage();
        ASBotFightLayer* aaa = ASBotFightLayer::createAsFightLayer(rolerTag,player1->rolerTag,isCritical,damageThisRound,2,player1->moveCount,player1->shengming,moveCount,shengming);
        aaa->setPosition(CCPointZero);
        getParent()->addChild(aaa,9999,9999);
    }
}

void ASBot::afterFightAnimation(){
    
    //1.无论如何对面都减血，清空行动力
    player1->changeHeart(-damageThisRound,1);
    moveCount = 0;
    changeMoveCount();
    changeEnergyBarAnimation(-100);
    combo = 0;
    isCritical = false;
    moveCountNeeded = moveToFight;
    
    //2.如果不是教学模式，并且这一刀对面也没有死的话，则:
    //(1)重新开始计时间（玩家答题时间，乌云，沉默，爆击药水)，机器人继续思考
    //(2)启用按钮（答题按钮，道具栏） 如果用户的行动力足够，则要帮用户启用技能按钮
    //(3)各种技能附带的效果生效
    if (!((GameBaseClass*)getParent())->isIntroducing && !((GameBaseClass*)getParent())->gameover) {
        
    /***********计时器***********/
        //2.0 答题与机器人
        player1->schedule(schedule_selector(ASGame::timeIsUp),0.04);
        botIsThinkingAboutAnswer();
        schedule(schedule_selector(ASBot::botReadyToEnterFightLayer), 5);
        botItemPlan();
  
        //2.1 乌云
        if (player1->clouded)
            schedule(schedule_selector(ASBot::countCloudTime),1);
        if(clouded)
            player1->schedule(schedule_selector(ASGame::countCloudTime),1);
        
        //2.2 沉默
        if (player1->silenceTime > 0)
            schedule(schedule_selector(ASBot::countSilenceTime),1);
        if (bot2->silenceTime > 0)
            player1->schedule(schedule_selector(ASGame::countSilenceTime),1);
        
        //2.3 爆击
        if (player1->criticalAddedTime > 0)
            player1->schedule(schedule_selector(ASGame::CriticalRateCounter));
        if (criticalAddedTime > 0)
            schedule(schedule_selector(ASBot::CriticalRateCounter));
    
    /***********启用按钮***********/
        
        //2.4 启用答题按钮
        CCMenu* optionMenu = (CCMenu*)player1->getChildByTag(1*10000000+12);
        optionMenu->setTouchEnabled(true);
        
        //2.5 启用道具栏
        CCMenu* itemMenu = (CCMenu*)player1->getChildByTag(1*10000000+77776);
        itemMenu->setTouchEnabled(true);
        
        //2.6 启用我方的大招按钮
        if (player1->moveCount >= player1->moveToFight) {
            CCMenu* menu = (CCMenu*)player1->getChildByTag(1*10000000+27);
            menu->setTouchEnabled(true);
        }
     
    /***********主动技能附加效果***********/
    
        //3.1 钢铁侠，附加乌云
        if(rolerTag == 1)
            coverQuestion(10+0.2*(Level-1));
        //3.2 初音，自身获得HOT
        else if(rolerTag == 2){
            hotRound = 2;
            botHotBuff();
        }
        //3.3 巫妖王，向对面施加DOT
        else if(rolerTag == 3 && player1->dotRound < 2){
            if (player1->dotRound == 0)
                dotAnimation();
            player1->dotRound = 2;
        }
        //3.4 谢耳朵，沉默对手
        else if(rolerTag == 4)
            botSilence();
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
        if (player1->rolerTag == 2 && player1->Level >= 10) //初音，刚烈
            botisGangLied();
    }
    
    
    //3.如果是教学模式，则释放乌云，然后下一步
    else if(((GameBaseClass*)getParent())->isIntroducing && !((GameBaseClass*)getParent())->gameover){
        coverQuestion(10+0.2*(Level-1));
        ((GameBaseClass*)getParent())->nextStep();
    }
    
    /*
    //4.如果这一刀先砍死对面，则胜利
    else if(((GameBaseClass*)getParent())->gameover){
        player1->unscheduleAllSelectors();
        unscheduleAllSelectors();
        ((GameBaseClass*)(this->getParent()))->GameResult(1);
    }
     */
}

