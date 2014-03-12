#include "AsChooseHero.h"
#include "ASUser.h"
#include "global.h"
extern ASUser* MainUser;

void AsChooseHero::updateGenderAndNickHTTP(){
    
    string message;
    if(gender!="" && nickName!="" && FirstHeroSelected!=-1){
        cout<<nickName<<endl;
        box = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
        box->setPosition(CCPointZero);
        addChild(box,10);
        
        CCHttpRequest* request = new CCHttpRequest();
        string UrlData = "http://115.29.168.228/users/update?user[gender]=" + gender + "&user[nick]=" + nickName + "&token=" + MainUser->UserTokenStr;
        request->setUrl(UrlData.c_str());
        request->setRequestType(CCHttpRequest::kHttpPut);
        request->setResponseCallback(this, callfuncND_selector(AsChooseHero::updateGenderAndNickRequestCompleted));
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
    else{
        if(FirstHeroSelected == -1)
            message = "请选择您的初始英雄";
        else if(gender == "")
            message = "请选择您的性别";
        else if(nickName == "")
            message = "请输入一个伟大的名字！";
        box = AsMessageBox::createMessageBox(message.c_str(), 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,10);
    }
}

void AsChooseHero::updateGenderAndNickRequestCompleted(CCNode* sender, void* data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    if (statusCode == 204){
        MainUser->gender = gender;
        MainUser->nickName = nickName;
        MainUser->HeroUnLocked[FirstHeroSelected] = true;
        unlockHeroHTTP(FirstHeroSelected);
    }
    else{
        box->closeMessageBox();
        box = AsMessageBox::createMessageBox("更新失败，请重试", 2, 1);
        box->setPosition(CCPointZero);
        addChild(box,10);
    }
}

void AsChooseHero::unlockHeroHTTP(int _HeroID){
    
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://115.29.168.228/roles");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, callfuncND_selector(AsChooseHero::unlockHeroRequestCompleted));
    string postDataStr = "role[avatar_id]=" + int2string(_HeroID) + "&role[level]=" + int2string(0) + "&token=" + MainUser->UserTokenStr;
    request->setRequestData(postDataStr.c_str(), postDataStr.length());
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void AsChooseHero::unlockHeroRequestCompleted(cocos2d::CCNode *sender, void *data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    if (statusCode == 201) {
        MainUser->hasChosenHero = true;
        box->closeMessageBox();
        box = AsMessageBox::createMessageBox("更新成功\n即将进入游戏", 2, 3);
        box->setPosition(CCPointZero);
        addChild(box,10);
        schedule(schedule_selector(AsChooseHero::goToTiBaoSelection),1);
    }
    else{
        box->closeMessageBox();
        box = AsMessageBox::createMessageBox("更新失败，请重试", 2, 3);
        box->setPosition(CCPointZero);
        addChild(box,10);
    }
}

void AsChooseHero::getRandomNameHTTP(){
    
    box = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
    box->setPosition(CCPointZero);
    addChild(box,1000);
    
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/random_nick?user[gender]=" + gender +  "&token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(AsChooseHero::getRandomNameRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void AsChooseHero::getRandomNameRequestCompleted(cocos2d::CCNode *sender, void *data){
        
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    box->closeMessageBox();
    nickName = "";
    for (unsigned int i = 0; i < buffer->size(); i++) {
        if ((*buffer)[i] == ':' && (*buffer)[i+1] == '"')
            for (int j = i + 2 ; (*buffer)[j] != '"' ; j++) {   nickName += (*buffer)[j]; }
    }
    randomNickName();
}

void AsChooseHero::printJson(std::vector<char> *buffer){
    for (unsigned int i = 0; i < buffer->size(); i++) {     cout<<(*buffer)[i];     }
    cout<<endl;
}
