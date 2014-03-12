#include "LogInScene.h"
#include "ASUser.h"

extern ASUser* MainUser;

void LogInScene::bindUserAccountHTTP(){
    
    closeLogInTable();
    
    if (UserNameStr != "" && PassWordStr !="") {
        box = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
        box->setPosition(CCPointZero);
        addChild(box,10);
        
        CCHttpRequest* request = new CCHttpRequest();
        string UrlData = "http://115.29.168.228/users/update?user[name]=" + UserNameStr + "&user[password]=" + PassWordStr + "&token=" + MainUser->UserTokenStr;
        request->setUrl(UrlData.c_str());
        request->setRequestType(CCHttpRequest::kHttpPut);
        request->setResponseCallback(this, callfuncND_selector(LogInScene::bindUserAccountRequestCompleted));
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
    else if(UserNameStr == ""){
        box = AsMessageBox::createMessageBox("请输入用户名", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
    else if(PassWordStr == ""){
        box = AsMessageBox::createMessageBox("请输入密码", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
}

void LogInScene::bindUserAccountRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    if (statusCode == 204) {
        //1.标记为已经登录
        MainUser->hasLogIn = true;
        //2.记录服务器发回的信息 TODO:服务器什么都没传回来
        MainUser->UserNameStr = UserNameStr;
        MainUser->PassWordStr = PassWordStr;
        
        cout<<"绑定候的登录信息为:"<<endl;
        cout<<"1.id:"<<MainUser->UserID<<endl;
        cout<<"2.用户名:"<<MainUser->UserNameStr<<endl;
        cout<<"3.密码:"<<MainUser->PassWordStr<<endl;
        cout<<"4.token:"<<MainUser->UserTokenStr<<endl;
        cout<<"5.金币数:"<<MainUser->gold<<endl;
        cout<<"6.性别:"<<MainUser->gender<<endl;
        cout<<"7.昵称:"<<MainUser->nickName<<endl;
        cout<<"8.是否已经进行过教学:"<<MainUser->introduced<<endl;
        
        saveLogInInformation(UserNameStr,PassWordStr);
        //3.关闭窗体
        box->closeMessageBox();
    }
    else if(statusCode == 422){
        box->closeMessageBox();
        box = AsMessageBox::createMessageBox("绑定错误，请重试", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,100);
    }
}