#include "LogInScene.h"
#include "global.h"
#include "LevelSelectionScene.h"
extern ASUser* MainUser;
void LogInScene::LogInHTTP(){
    
    closeLogInTable();
    
    if (UserNameStr != "" && PassWordStr !="" && ((UserNameStr != MainUser->UserNameStr && MainUser->hasLogIn) || !MainUser->hasLogIn)) {
        
        box = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
        box->setPosition(CCPointZero);
        addChild(box,100);
        
        CCHttpRequest* request = new CCHttpRequest();
        string UrlData = "http://115.29.168.228/users/signin?user[name]=" + UserNameStr + "&user[password]=" + PassWordStr;
        request->setUrl(UrlData.c_str());
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback(this, callfuncND_selector(LogInScene::LogInRequestCompleted));
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
    else if(UserNameStr == ""){
        box = AsMessageBox::createMessageBox("请输入用户名", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
    else if(UserNameStr == MainUser->UserNameStr){
        box = AsMessageBox::createMessageBox("该账户已经登录", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
    else if(PassWordStr == ""){
        box = AsMessageBox::createMessageBox("请输入密码", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
}

void LogInScene::LogInRequestCompleted(CCNode* sender,void* data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    if (statusCode == 200) {
        //1.标记为已经登录
        if (!MainUser->hasLogIn) {
            MainUser->hasLogIn = true;
            BeginMenuFallDown();  //关闭网络，进入游戏，没有按钮掉下来，再打开网络，重新登录以后，要掉下来
        }
        //2.记录服务器发回的信息
        if(!MainUser->initUser(buffer))
            MainUser->hasChosenHero = false;
        else
            MainUser->hasChosenHero = true;
        saveLogInInformation(UserNameStr,PassWordStr);
        //3.初始化rank
        tmpBoardID = 0;
        getUserHistoryGame(tmpBoardID);
    }
    else{
        if(statusCode == 401){
            box->closeMessageBox();
            box = AsMessageBox::createMessageBox("用户名与密码不匹配", 2, 1);
            box->setPosition(CCPointZero);
            addChild(box,100);
        }
        else if(statusCode == 404){
            box->closeMessageBox();
            box = AsMessageBox::createMessageBox("该用户不存在", 2, 1);
            box->setPosition(CCPointZero);
            addChild(box,100);
        }
        else{
            box->closeMessageBox();
            box = AsMessageBox::createMessageBox("无法连接，请检查您的网络", 2, 1);
            box->setPosition(CCPointZero);
            addChild(box,100);
        }
    }
}

void LogInScene::getUserHistoryGame(int _index){
    if(_index<=14){
        CCHttpRequest* request = new CCHttpRequest();
        string UrlData = "http://115.29.168.228/winning_boards/" + int2string(_index) + "/whereami?token=" + MainUser->UserTokenStr;
        request->setUrl(UrlData.c_str());
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback(this, callfuncND_selector(LogInScene::getUserHistoryGameRequestCompleted));
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
}

void LogInScene::getUserHistoryGameRequestCompleted(CCNode* sender,void* data){
    
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
        cout<<_id<<":胜"<<WinTimes<<" 负"<<LoseTimes<<" Rank为"<<MainUser->Rank[tmpBoardID]<<endl;
    }
    
    //3.继续下一个题的rank
    if (tmpBoardID < 14) {
        tmpBoardID++;
        boardNotFound = false;
        getUserHistoryGame(tmpBoardID);
    }
    else
        box->closeMessageBox();
}

void LogInScene::printJson(std::vector<char> *buffer){
    for (unsigned int i = 0; i < buffer->size(); i++) {     cout<<(*buffer)[i];     }
    cout<<endl;
}



