#include "ASUser.h"

void ASUser::getHeroInformationHTTP(){
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/roles?token=" + UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(ASUser::getHeroInformationRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void ASUser::getHeroInformationRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    for (int i = 0 ; i < 5 ; i++) {
        HeroLevel[i] = 0;
        HeroUnLocked[i] = false;
    }

    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    for (unsigned int i = 0; i < buffer->size(); i++) {
        //1.1 第1个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '0' && (*buffer)[i+14] == ','){
            HeroUnLocked[0] = true;
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  HeroLevelStr[0] += (*buffer)[j]; }
        }
        //1.2 第2个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '1' && (*buffer)[i+14] == ','){
            HeroUnLocked[1] = true;
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  HeroLevelStr[1] += (*buffer)[j]; }
        }
        //1.3 第3个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '2' && (*buffer)[i+14] == ','){
            HeroUnLocked[2] = true;
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  HeroLevelStr[2] += (*buffer)[j]; }
        }
        //1.4 第4个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '3' && (*buffer)[i+14] == ','){
            HeroUnLocked[3] = true;
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  HeroLevelStr[3] += (*buffer)[j]; }
        }
        //1.5 第5个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '4' && (*buffer)[i+14] == ','){
            HeroUnLocked[4] = true;
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  HeroLevelStr[4] += (*buffer)[j]; }
        }
    }
    
    for (int i = 0 ; i < 5 ; i++) {
        HeroLevel[i] = atoi(HeroLevelStr[i].c_str())+1;
        cout<<HeroLevel[i]<<" ";
    }
    cout<<endl;
    for (int i = 0 ; i < 5 ; i++) {
        cout<<HeroUnLocked[i]<<" ";
    }
    cout<<endl;
}

void ASUser::printJson(std::vector<char> *buffer){
    for (unsigned int i = 0; i < buffer->size(); i++) {     cout<<(*buffer)[i];     }
    cout<<endl;
}

