#include "GameBaseClass.h"
#include "ASUser.h"
#include "global.h"
#include "ASBot.h"
#include "ASGame.h"

extern ASUser* MainUser;
extern ASGame* player1;
extern ASBot* bot2;
extern int tibaoType;

/***************************************************************用户的：胜负、总答题数、正确题数***************************************************************/
void GameBaseClass::updateUserWinHTTP(){
    
    gameover = false;  //关闭掉这个状态，不然一直点击结尾就会重新刷congscene

    box = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
    box->setPosition(CCPointZero);
    addChild(box,10002);
    
    string UrlData = "http://115.29.168.228/winning_boards/";
    if (userSuccess == 0) //用户胜利
        UrlData+= int2string(tibaoType) + "/win";
    else if(userSuccess == 1)//用户失败
        UrlData+= int2string(tibaoType) + "/lose";
    
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, callfuncND_selector(GameBaseClass::updateUserWinRequestCompleted));
    string postDataStr = "token=" + MainUser->UserTokenStr + "&total_answer_count" + int2string(player1->rightNumber + player1->wrongNumber) + "&correct_answer_count" + int2string(player1->rightNumber);
    request->setRequestData(postDataStr.c_str(), postDataStr.length());
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameBaseClass::updateUserWinRequestCompleted(cocos2d::CCNode *sender, void *data){
    
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
    
    //2.计算新的rank
    MainNewRank = MainUser->calculateRank(tibaoType,atoi(WinTimesStr.c_str()),atoi(LoseTimesStr.c_str()));
    cout<<"旧rank为"<<MainUser->Rank[tibaoType]<<" 新rank为"<<MainNewRank<<endl;
    
    updateUserGoldHTTP();
}

/***************************************************************用户的：Rank************************************************************************************/
/*
void GameBaseClass::updateUserRankHTTP(){
    
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/update?user[rank_score]=" + int2string(MainRank) + "&token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpPut);
    request->setResponseCallback(this, callfuncND_selector(GameBaseClass::updateUserRankRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameBaseClass::updateUserRankRequestCompleted(cocos2d::CCNode *sender, void *data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    updateUserGoldHTTP();
}
 */

/***************************************************************用户的：金币************************************************************************************/
void GameBaseClass::updateUserGoldHTTP(){
    
    userGoldPlus = 0;
    
    //1.计算更新后的金币
    if (userSuccess == 0) {
        userGoldPlus+= MainNewRank/15.;
        if (MainNewRank < bot2->botRank)
            userGoldPlus += 200;
        else
            userGoldPlus += 150;
        userGoldPlus *= (1+ 0.01*(player1->Level));
    }
    else if(userSuccess == 1)
        userGoldPlus += 50;
    
    MainUser->gold += userGoldPlus;
    cout<<"用户：金币加了"<<userGoldPlus<<",更新后的金币为"<<MainUser->gold<<endl;

    //2.上传更新后的金币
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/update?&user[gold]=" + int2string(MainUser->gold) + "&token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpPut);
    request->setResponseCallback(this, callfuncND_selector(GameBaseClass::updateUserGoldRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameBaseClass::updateUserGoldRequestCompleted(cocos2d::CCNode *sender, void *data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    updateBotWinHTTP();
}

void GameBaseClass::printJson(std::vector<char> *buffer){
    for (unsigned int i = 0; i < buffer->size(); i++) {     cout<<(*buffer)[i];     }
    cout<<endl;
}

