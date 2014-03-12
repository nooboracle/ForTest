#include "LoadingScene.h"
#include "ASUser.h"
#include "LogInScene.h"
#include "global.h"
extern ASUser* MainUser;

void LoadingScene::LogInHTTP(){
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/signin?user[name]=" + MainUser->UserNameStr + "&user[password]=" + MainUser->PassWordStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(LoadingScene::LogInRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void LoadingScene::LogInRequestCompleted(CCNode* sender,void* data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    if (statusCode == 200) {
        MainUser->hasLogIn = true;
        if(MainUser->initUser(buffer))
            MainUser->hasChosenHero = true;
        getUserHistoryGame(tmpBoardID);
    }
    else if(statusCode != 200){
        CCMessageBox("请检查您的网络\n或重新输入用户名及密码", "登录失败");
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LogInScene::scene()));
    }
}

void LoadingScene::FastSignUp(){
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://115.29.168.228/users/signup");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, callfuncND_selector(LoadingScene::FastSignUpRequestCompleted));
    string postDataStr = "";
    request->setRequestData(postDataStr.c_str(), 0);
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void LoadingScene::FastSignUpRequestCompleted(CCNode* sender, void* data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    //0.状态标记为已经登录
    MainUser->hasLogIn = true;
    //1.初始化账号信息
    MainUser->initUser(buffer);
    //2.初始化rank
    for (int i = 0 ; i <= 14; i++)
        MainUser->Rank[i] = 0;
    //3.将账号和密码存入本地数据库
    saveLogInInformation(MainUser->UserNameStr,MainUser->PassWordStr);
    //4.下一个页面
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LogInScene::scene()));
}

void LoadingScene::getUserHistoryGame(int _index){
    if(_index<=14){
        CCHttpRequest* request = new CCHttpRequest();
        string UrlData = "http://115.29.168.228/winning_boards/" + int2string(_index) + "/whereami?token=" + MainUser->UserTokenStr;
        request->setUrl(UrlData.c_str());
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback(this, callfuncND_selector(LoadingScene::getUserHistoryGameRequestCompleted));
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
}

void LoadingScene::getUserHistoryGameRequestCompleted(CCNode* sender,void* data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    //1.解析
    string boardIdStr,WinTimesStr,LoseTimesStr;
    for (unsigned int i = 0; i < buffer->size(); i++) {
        //1.排行榜编号
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'b' && (*buffer)[i+2] == 'o' && (*buffer)[i+3] == 'a' && (*buffer)[i+4] == 'r') {
            for (int j = i + 11; (*buffer)[j] != ','; j++) {   boardIdStr += (*buffer)[j];    }
        }
        //2.历史胜场
        else if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'h' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 's' && (*buffer)[i+4] == 't' && (*buffer)[i+9] == 'w' && (*buffer)[i+12] == '"') {
            for (int j = i + 14; (*buffer)[j] != ','; j++) {   WinTimesStr += (*buffer)[j];    }
        }
        //3.历史败场
        else if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'h' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 's' && (*buffer)[i+4] == 't' && (*buffer)[i+9] == 'l' && (*buffer)[i+13] == '"') {
            for (int j = i + 15; (*buffer)[j] != ','; j++) {   LoseTimesStr += (*buffer)[j];    }
        }
        //4.如果还没玩过这个
        if ((*buffer)[i] == '4' && (*buffer)[i+1] == '0' && (*buffer)[i+2] == '4') {
            boardNotFound = true;
            break;
        }
    }
    
    //2.计算rank
    if (boardNotFound)
        MainUser->Rank[tmpBoardID] = 0;
    else{
        int _id = atoi(boardIdStr.c_str());
        int WinTimes = atoi(WinTimesStr.c_str());
        int LoseTimes = atoi(LoseTimesStr.c_str());
        MainUser->Rank[tmpBoardID] = MainUser->calculateRank(_id,WinTimes,LoseTimes);
        cout<<_id<<":胜"<<WinTimes<<" 负"<<LoseTimes<<" Rank为"<<MainUser->calculateRank(_id,WinTimes,LoseTimes)<<endl;
    }
    
    //3.继续下一个题的rank
    if (tmpBoardID < 14) {
        tmpBoardID++;
        boardNotFound = false;
        getUserHistoryGame(tmpBoardID);
    }
    else
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LogInScene::scene()));
}

void LoadingScene::printJson(std::vector<char> *buffer){
    for (unsigned int i = 0; i < buffer->size(); i++) {     cout<<(*buffer)[i];     }
    cout<<endl;
}



