#include "RankScene.h"
#include "ASUser.h"
#include "global.h"

extern ASUser* MainUser;
extern int djSelected[3];
extern int djPrice[7];

void RankScene::updateUserGold(){

    if (goldProcessingIndex == 1 || goldProcessingIndex == 3 || goldProcessingIndex == 4)
        MessageBox->closeMessageBox();

    MessageBox = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
    MessageBox->setPosition(CCPointZero);
    addChild(MessageBox,10001);
    
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/update?&user[gold]=" + int2string(MainUser->gold) + "&token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpPut);
    request->setResponseCallback(this, callfuncND_selector(RankScene::updateUserGoldRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::updateUserGoldRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    if (goldProcessingIndex == 1 && statusCode == 204) {
        
        MainUser->energyNumber += 1;
        
        updateUserEnergy();
        
    }else if(goldProcessingIndex == 1 && statusCode != 204) {
        
        MessageBox = AsMessageBox::createMessageBox("通信失败\n请检查您的网络", 2, 1);
        MessageBox->setPosition(CCPointZero);
        addChild(MessageBox,1000);
        
        changeGold(120);
        
    }else if(goldProcessingIndex == 2 && statusCode == 204) {
        
        MessageBox->closeMessageBox();
        openMatchLayer();
        
    }else if(goldProcessingIndex == 2 && statusCode != 204) {
        
        MessageBox->closeMessageBox();
        MessageBox = AsMessageBox::createMessageBox("通信失败\n请检查您的网络", 2, 1);
        MessageBox->setPosition(CCPointZero);
        addChild(MessageBox,1000);
        
        //for (int i = 0 ; i < 3 ; i++){
          //  if (djSelected[i]!=0)
            //    changeGold(djPrice[djSelected[i]-1]);
        //}
    }
    else if(goldProcessingIndex == 3 || goldProcessingIndex == 4)
        MessageBox->closeMessageBox();
}

void RankScene::updateUserEnergy(){
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/update?user[life]=" + int2string(MainUser->energyNumber) + "&token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpPut);
    request->setResponseCallback(this, callfuncND_selector(RankScene::updateUserEnergyRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::updateUserEnergyRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    MessageBox->closeMessageBox();
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    for (int i = MainUser->energyNumber-1 ; i < MainUser->energyNumber ; i++) {
        CCSprite* energy = CCSprite::createWithSpriteFrameName("heart.png");
        energy->setPosition(ccp(size.width*10/80+i*size.width*12.7/240, size.height*2100/2200+winDif*4*(2-alpha)));
        addChild(energy,10000,41+i);
    }
}

void RankScene::MinusEnergyHTTP(){
    CCHttpRequest* request = new CCHttpRequest();
    string urlStr = "http://115.29.168.228/users/life?token=" + MainUser->UserTokenStr;
    request->setUrl(urlStr.c_str());
    request->setRequestType(CCHttpRequest::kHttpDelete);
    request->setResponseCallback(this, callfuncND_selector(RankScene::MinusEnergyRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::MinusEnergyRequestCompleted(cocos2d::CCNode *sender, void *data){
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    schedule(schedule_selector(RankScene::BeginOnLineGame), 1.5);
}

void RankScene::getEnergyHTTP(){
    
    MessageBox = AsMessageBox::createMessageBox("通信中，请稍候", 1, 0);
    MessageBox->setPosition(CCPointZero);
    addChild(MessageBox,100001);
    
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/life?token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankScene::getEnergyRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::getEnergyRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    //1.重置存储用的string
    year_next = month_next = day_next = clock_next = minute_next = second_next = "" ;
    
    //2.获得服务器返回的字符串
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    //3.解析
    for (unsigned int i = 0; i < buffer->size(); i++) {
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'l' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 'f' && (*buffer)[i+4] == 'e' && (*buffer)[i+5] == '"' && (*buffer)[i+6] == ':') {
            for (int j = i + 7 ; (*buffer)[j] != ',' ; j++) {   energyStr += (*buffer)[j]; }
        }
        if((*buffer)[i] == '"' && (*buffer)[i+1] == 'l' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 'f' && (*buffer)[i+4] == 'e' && (*buffer)[i+5] == '_' && (*buffer)[i+6] == 'r' && (*buffer)[i+16] == '_' && (*buffer)[i+21]!='n') {
            for (int j = 0 ; j < 4 ; j++)
                year_next += (*buffer)[i+22+j];
            for (int j = 0 ; j < 2 ; j++){
                month_next += (*buffer)[i+27+j];
                day_next += (*buffer)[i+30+j];
                clock_next += (*buffer)[i+33+j];
                minute_next += (*buffer)[i+36+j];
                second_next += (*buffer)[i+39+j];
            }
            break;
        }
    }
    
    //4.记录当前的体力数
    MainUser->energyNumber = atoi(energyStr.c_str());

    //5.关闭弹出层
    MessageBox->closeMessageBox();
    
    //6.继续获取排行榜列表
    getRankListHTTP();
}