#include "GameBaseClass.h"
#include "ASUser.h"
#include "global.h"
#include "ASBot.h"
#include "ASGame.h"

extern ASUser* MainUser;
extern ASGame* player1;
extern ASBot* bot2;
extern int tibaoType;

void GameBaseClass::updateBotWinHTTP(){
    
    string UrlData = "http://115.29.168.228/winning_boards/";
    if (userSuccess == 0) //用户胜利,机器人失败
        UrlData+= int2string(tibaoType) + "/lose";
    else if(userSuccess == 1)//用户失败,机器人胜利
        UrlData+= int2string(tibaoType) + "/win";
    
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, callfuncND_selector(GameBaseClass::updateBotWinRequestCompleted));
    string postDataStr = "token=" + bot2->botToken;
    request->setRequestData(postDataStr.c_str(), postDataStr.length());
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameBaseClass::updateBotWinRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    //1.解析
    string boardIdStr,WinTimesStr,LoseTimesStr;
    for (unsigned int i = 0; i < buffer->size(); i++) {
        //1.1 历史胜场
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'h' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 's' && (*buffer)[i+4] == 't' && (*buffer)[i+9] == 'w' && (*buffer)[i+12] == '"')
            for (int j = i + 14; (*buffer)[j] != ','; j++) {   WinTimesStr += (*buffer)[j];    }
        //1.2 历史败场
        else if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'h' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 's' && (*buffer)[i+4] == 't' && (*buffer)[i+9] == 'l' && (*buffer)[i+13] == '"')
            for (int j = i + 15; (*buffer)[j] != ','; j++) {   LoseTimesStr += (*buffer)[j];    }
    }
    BotWinTimesHistory = WinTimesStr.c_str();
    cout<<"机器人的累计胜场为"<<BotWinTimesHistory<<endl;
    
    //2.计算新的rank = 基础分 ＋ 胜率基本分*胜场系数*1.2 ＋ 胜场分
    int botWinTimes = atoi(WinTimesStr.c_str());
    int botLoseTimes = atoi(LoseTimesStr.c_str());
    if(botWinTimes + botLoseTimes <= 10)
        BotNewRank = 100*(botWinTimes + botLoseTimes);
    else{
        int botWinPercent = (float)botWinTimes/((float)botWinTimes + (float)botLoseTimes);
        //计算rank ＝ 基础分 ＋ 胜率基本分*胜场系数*1.2 ＋ 胜场分
        //2.1 基本分
        int BaseRankScore = 1000;
        //2.2 胜率分 ＝ 胜率基本分*胜场系数*1.2
        int BaseWinPercentScore,WinPercentScore;
        if (botWinPercent*100 >= 50)
            BaseWinPercentScore = 750 + 100* pow((double)(botWinPercent*100 - 50),0.7);
        else
            BaseWinPercentScore = 750 - 100* pow((double)(50 - botWinPercent*100),0.7);
        
        if (botWinTimes < 20)
            WinPercentScore = BaseWinPercentScore*1.2*0.5;
        else if(botWinTimes >= 20 && botWinTimes < 50)
            WinPercentScore = BaseWinPercentScore*1*1.2;
        else if(botWinTimes >= 50 && botWinTimes < 100)
            WinPercentScore = BaseWinPercentScore*1.1*1.2;
        else if(botWinTimes >= 100 && botWinTimes < 150)
            WinPercentScore = BaseWinPercentScore*1.15*1.2;
        else if(botWinTimes >= 150 && botWinTimes < 300)
            WinPercentScore = BaseWinPercentScore*1.2*1.2;
        else if(botWinTimes >= 300 && botWinTimes < 500)
            WinPercentScore = BaseWinPercentScore*1.25*1.2;
        else if(botWinTimes >= 500)
            WinPercentScore = BaseWinPercentScore*1.3*1.2;
        //2.3 胜场分
        int WinTimesScore = botWinTimes*3 + (BaseRankScore-900)*1.5;
        if(WinTimesScore >= 3500)
            WinTimesScore = 3500;
        //2.4 总分
        BotNewRank = BaseRankScore + WinPercentScore + WinTimesScore;
        if (BotNewRank < 0)
            BotNewRank = 0;
    }

    //3.上传新的金币数
    updateBotGoldHTTP();
}

void GameBaseClass::updateBotGoldHTTP(){
    
    botGoldPlus = 0;

    //1.计算更新后的金币
    if (userSuccess == 1) {
        botGoldPlus += BotNewRank/15.;
        if (BotNewRank < MainNewRank)
            botGoldPlus += 200;
        else
            botGoldPlus += 150;
        botGoldPlus *= (1+ 0.01*(bot2->Level));
    }
    else if(userSuccess == 0)
        botGoldPlus += 50;
    
    bot2->gold += botGoldPlus;
    cout<<"机器人：金币加了"<<botGoldPlus<<",更新后的金币为"<<bot2->gold<<endl;
    
    //2.计算是否有角色可以升级
    int goldToLevelUp[50];
    for (int i = 0 ; i < 5 ; i++) {
        if (bot2->HeroLevel[i]+1 == 9)
            goldToLevelUp[i] = 3500;
        else
            goldToLevelUp[i] = (bot2->HeroLevel[i]+1)*100;
        if (goldToLevelUp[i] <= bot2->gold) {
            botLevelUpIndex = i;
            cout<<"升级的机器人编号为"<<botLevelUpIndex<<"+1"<<",消耗金币为"<<goldToLevelUp[i];
            bot2->gold -= goldToLevelUp[i];
            cout<<"更新后的金币为"<<bot2->gold<<endl;
            bot2->HeroLevel[i]+=1;
            break;
        }
    }
    cout<<"前5个英雄的等级分别为"<<bot2->HeroLevel[0]<<" "<<bot2->HeroLevel[1]<<" "<<bot2->HeroLevel[2]<<" "<<bot2->HeroLevel[3]<<" "<<bot2->HeroLevel[4]<<endl;

    //3.上传机器人更新后的金币数
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/update?&user[gold]=" + int2string(bot2->gold) + "&token=" + bot2->botToken;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpPut);
    request->setResponseCallback(this, callfuncND_selector(GameBaseClass::updateBotGoldRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameBaseClass::updateBotGoldRequestCompleted(cocos2d::CCNode *sender, void *data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    updateBotHeroLevel();
}

void GameBaseClass::updateBotHeroLevel(){
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://115.29.168.228/roles");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, callfuncND_selector(GameBaseClass::updateBotHeroLevelRequestCompleted));
    string postDataStr = "role[avatar_id]=" + int2string(botLevelUpIndex) + "&role[level]=" + int2string(bot2->HeroLevel[botLevelUpIndex]) + "&token=" + bot2->botToken;
    request->setRequestData(postDataStr.c_str(), postDataStr.length());
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameBaseClass::updateBotHeroLevelRequestCompleted(cocos2d::CCNode *sender, void *data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    releaseRobot();
}

void GameBaseClass::releaseRobot(){
    CCHttpRequest* request = new CCHttpRequest();
    string urlStr = "http://115.29.168.228/robots/" + bot2->botID + "?token=" + bot2->botToken;
    request->setUrl(urlStr.c_str());
    request->setRequestType(CCHttpRequest::kHttpDelete);
    request->setResponseCallback(this, callfuncND_selector(GameBaseClass::releaseRobotCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameBaseClass::releaseRobotCompleted(cocos2d::CCNode *sender, void *data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);

    box->closeMessageBox();
    CongScene();
}

