#include "RankScene.h"
#include "global.h"
#include "ASUser.h"

extern ASUser* MainUser;

void RankScene::HeroLevelUpHTTP(int _HeroIndex,int _HeroLevel){
    
    MessageBox = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
    MessageBox->setPosition(CCPointZero);
    addChild(MessageBox,1000);
    
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/roles/" + int2string(_HeroIndex) + "?role[level]=" + int2string(_HeroLevel) + "&token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpPut);
    request->setResponseCallback(this, callfuncND_selector(RankScene::HeroLevelUpRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}
void RankScene::HeroLevelUpRequestCompleted(CCNode* sender,void* data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    if (statusCode == 204) {
        changeGold(-MainUser->HeroLevelUpGold[MainUser->HeroLevel[roleIndex-1]-1]);
        MainUser->HeroLevel[roleIndex-1]++;
        LevelUpAnimation(roleIndex-1);
        
        goldProcessingIndex = 4;
        updateUserGold();
    }
    else{
        MessageBox->closeMessageBox();
        MessageBox = AsMessageBox::createMessageBox("与服务器通信失败，\n请检查您的网络", 2, 1);
        MessageBox->setPosition(CCPointZero);
        addChild(MessageBox,1000);
    }
}

void RankScene::unlockHeroHTTP(int _HeroIndex){
    
    MessageBox = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
    MessageBox->setPosition(CCPointZero);
    addChild(MessageBox,1000);
    
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://115.29.168.228/roles");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, callfuncND_selector(RankScene::unlockHeroRequestCompleted));
    string postDataStr = "role[avatar_id]=" + int2string(_HeroIndex) + "&role[level]=" + int2string(0) + "&token=" + MainUser->UserTokenStr;
    request->setRequestData(postDataStr.c_str(), postDataStr.length());
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::unlockHeroRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    if (statusCode == 201) {
        changeGold(-MainUser->HeroUnLockPrice[HeroProcessing]);
        MainUser->HeroUnLocked[HeroProcessing] = true;
        UnlockAnimation(HeroProcessing);
        goldProcessingIndex = 4;
        updateUserGold();
    }
    else{
        MessageBox->closeMessageBox();
        MessageBox = AsMessageBox::createMessageBox("与服务器通信失败，\n请检查您的网络", 2, 1);
        MessageBox->setPosition(CCPointZero);
        addChild(MessageBox,1000);
    }
}