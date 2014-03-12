#include "LogInScene.h"
#include "ASUser.h"
extern ASUser* MainUser;
void LogInScene::SignUpHTTP(){
    
    closeLogInTable();
    
    if(UserNameStr != "" && PassWordStr.length() >= 8){
        
        box = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
        box->setPosition(CCPointZero);
        addChild(box,100);
        
        CCHttpRequest* request = new CCHttpRequest();
        request->setUrl("http://115.29.168.228/users/signup");
        request->setRequestType(CCHttpRequest::kHttpPost);
        request->setResponseCallback(this, callfuncND_selector(LogInScene::SignUpRequestCompleted));
        string postDataStr = "user[name]=" + UserNameStr + "&user[password]=" + PassWordStr;
        request->setRequestData(postDataStr.c_str(), postDataStr.length());
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
    else if(UserNameStr == ""){
        box = AsMessageBox::createMessageBox("您还没输入用户名", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
    else if(PassWordStr == ""){
        box = AsMessageBox::createMessageBox("您还没输入密码", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
    else if(PassWordStr.length() < 8){
        box = AsMessageBox::createMessageBox("密码至少为8位字符", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
}

void LogInScene::SignUpRequestCompleted(CCNode* sender, void* data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    if (statusCode == 201) {
        //1.状态标记为已经登录
        MainUser->hasLogIn = true;
        //2.初始化账户信息
        if(!MainUser->initUser(buffer))
            MainUser->hasChosenHero = false;
        saveLogInInformation(UserNameStr,PassWordStr);
        //3.初始化rank
        for (int i = 0 ; i <= 14; i++)
            MainUser->Rank[i] = 0;
        //4.提示
        box->closeMessageBox();
        box = AsMessageBox::createMessageBox("恭喜,注册成功", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
    else if(statusCode != 201){
        box->closeMessageBox();
        box = AsMessageBox::createMessageBox("注册失败，请重试", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
}