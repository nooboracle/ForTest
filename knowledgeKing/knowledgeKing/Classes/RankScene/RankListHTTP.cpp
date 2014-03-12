#include "RankScene.h"
#include "ASUser.h"
#include "global.h"
#include "AsMessageBox.h"
using namespace std;

extern ASUser* MainUser;
extern string tmpBotID;
extern int tibaoType;

void RankScene::getRankListHTTP(){
    //0.重置所有信息,否则会一直加
    for (int i = 0 ; i < 20 ; i++) {
        nickName_Rank[i] = "";
        winPercent_Rank[i] = "";
        weeklyWin_Rank[i] = "";
    }
    //2.显示提示框
    MessageBox = AsMessageBox::createMessageBox("正在获取排行榜信息，请稍候", 1,0);
    MessageBox->setPosition(CCPointZero);
    addChild(MessageBox,10001);
    //3.开始向服务器请求信息
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/winning_boards/" + int2string(tibaoType) + "/weekly?token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankScene::getRankListRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::getRankListRequestCompleted(CCNode* sender,void* data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    int counter_name = 0;
    int counter_winPercent = 0;
    int counter_weeklywin = 0;
    
    for (unsigned int i = 0; i < buffer->size(); i++) {
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'n' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 'c' && (*buffer)[i+4] == 'k' && (*buffer)[i+5] == '"') {
            for (int j = i + 8 ; (*buffer)[j] != '"' ; j++) {   nickName_Rank[counter_name] += (*buffer)[j]; }
            counter_name++;
        }
        if((*buffer)[i] == '"' && (*buffer)[i+1] == 'w' && (*buffer)[i+2] == 'e' && (*buffer)[i+3] == 'e' && (*buffer)[i+4] == 'k' && (*buffer)[i+5] == 'l' && (*buffer)[i+19] == '"') {
            for (int j = i + 21 ; (*buffer)[j] != ',' ; j++) {   winPercent_Rank[counter_winPercent] += (*buffer)[j]; }
            winPercent_Rank[counter_winPercent] = float2string(atof(winPercent_Rank[counter_winPercent].c_str())*100) + "%";
            counter_winPercent++;
        }
        if((*buffer)[i] == '"' && (*buffer)[i+1] == 'w' && (*buffer)[i+2] == 'e' && (*buffer)[i+3] == 'e' && (*buffer)[i+4] == 'k' && (*buffer)[i+5] == 'l' && (*buffer)[i+11] == '"') {
            for (int j = i + 13 ; (*buffer)[j] != ',' ; j++) {   weeklyWin_Rank[counter_weeklywin] += (*buffer)[j]; }
            counter_weeklywin++;
        }
    }
    
    getServerTime();
}

void RankScene::getServerTime(){
    
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/time";
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankScene::getServerTimeRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::getServerTimeRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    year_current = month_current = day_current = clock_current = minute_current = second_current = "";
    
    //1.获取服务器返回的时间信息
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    //2.解析时间信息
    string expStr;
    int now,exp;
    for (unsigned int i = 0; i < buffer->size(); i++) {
        //2.1 当前时间
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'n' && (*buffer)[i+2] == 'o' && (*buffer)[i+3] == 'w' && (*buffer)[i+4] == '"' && (*buffer)[i+5] == ':'){
            for (int j = 0 ; j < 4 ; j++)
                year_current += (*buffer)[i+7+j];
            for (int j = 0 ; j < 2 ; j++){
                month_current += (*buffer)[i+12+j];
                day_current += (*buffer)[i+15+j];
                clock_current += (*buffer)[i+18+j];
                minute_current += (*buffer)[i+21+j];
                second_current += (*buffer)[i+24+j];
            }
        }
        //2.2 结算时间
        if((*buffer)[i] == 'a' && (*buffer)[i+1] == 't' && (*buffer)[i+2] == '"' && (*buffer)[i+3] == ':' && (*buffer)[i+4] == '"' && (*buffer)[i+5] == '2')
            for (int j = 0;  j < 2 ; j++)
                expStr += (*buffer)[i+13+j];
    }
    
    //3.计算下次结算时间
    int daysPerMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (day_current[0] == 0)
        now = atoi(&day_current[1]);
    else
        now = atoi(day_current.c_str());
    if (expStr[0] == 0)
        exp = atoi(&expStr[1]);
    else
        exp = atoi(expStr.c_str());
    if (exp < now) {
        serverTime = exp + daysPerMonth[atoi(month_current.c_str())-1] - now;
    }else
        serverTime = exp - now;
    
    //4.计算下次回复时间,总的秒数
    int nextSecond = 0;
    if (year_next!="" && month_next!="" && day_next!="" && clock_next!="" && minute_next!="" && second_next!="") {
        nextSecond += atoi(year_next.c_str())*31536000;
        for (int i = 0 ; i < atoi(month_next.c_str()); i++)
            nextSecond += daysPerMonth[i]*24*60*60;
        nextSecond += atoi(day_next.c_str())*24*60*60;
        nextSecond += atoi(clock_next.c_str())*60*60;
        nextSecond += atoi(minute_next.c_str())*60;
        nextSecond += atoi(second_next.c_str());
    }else
        nextSecond = 0;
    
    //5.计算当前的时间，总的秒数
    int currentSecond = 0;
    if (nextSecond!=0) {
        currentSecond += atoi(year_current.c_str())*31536000;
        for (int i = 0 ; i < atoi(month_current.c_str()); i++)
            currentSecond += daysPerMonth[i]*24*60*60;
        currentSecond += atoi(day_current.c_str())*24*60*60;
        currentSecond += atoi(clock_current.c_str())*60*60;
        currentSecond += atoi(minute_current.c_str())*60;
        currentSecond += atoi(second_current.c_str());
    }else
        currentSecond = 0;
        
    nextRecoverySecond = nextSecond - currentSecond;
    
    //6.体力部分
    for (int i = 0 ; i < 6 ; i++)
        removeChildByTag(41+i);

    for (int i = 0 ; i < MainUser->energyNumber ; i++) {
        CCSprite* energy = CCSprite::createWithSpriteFrameName("heart.png");
        energy->setPosition(ccp(size.width*10/80+i*size.width*12.7/240, size.height*2100/2200+winDif*4*(2-alpha)));
        addChild(energy,10000,41+i);
    }
    
    //7.体力回复时间
    if (MainUser->energyNumber < 5) {
        string nextRecoveryTimStr = int2string(nextRecoverySecond/60) + ":" + int2string(nextRecoverySecond%60);
        CCLabelTTF* recoveryTime = CCLabelTTF::create(nextRecoveryTimStr.c_str(), "Arial Rounded MT Bold", 24,CCSizeMake(size.width/3, 0),kCCTextAlignmentRight);
        recoveryTime->setPosition(ccp(size.width*20/80,size.height*2100/2200+winDif*4*(2-alpha)));
        addChild(recoveryTime,10000,46);
        
        schedule(schedule_selector(RankScene::energyTimeCounter), 1);
    }

    //5.关闭提示框
    MessageBox->closeMessageBox();
    
    //6.下一个动作
    if (tableIndex == 1) //如果之前就是排行榜，则直接把信息写上去
        rankListContent();
    else if(tableIndex == 2)//如果之前是题包列表，则旋转
        goToRankList();
}

void RankScene::getRankHTTP(){
    
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/winning_boards/" + int2string(tibaoType) + "/whereami?token=" + MainUser->UserTokenStr;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankScene::getRankRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::getRankRequestCompleted(CCNode* sender,void* data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
}

void RankScene::printJson(std::vector<char> *buffer){
    for (unsigned int i = 0; i < buffer->size(); i++) {     cout<<(*buffer)[i];     }
    cout<<endl;
}