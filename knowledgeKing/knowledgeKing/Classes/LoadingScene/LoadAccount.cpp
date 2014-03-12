#include "LoadingScene.h"
#include "ASUser.h"
#include "LogInScene.h"
using namespace std;

extern ASUser* MainUser;

void LoadingScene::saveLogInInformation(string _name,string _password){
    
    CCUserDefault::sharedUserDefault()->setStringForKey("name_", _name);
    CCUserDefault::sharedUserDefault()->setStringForKey("psw_", _password);
}

int LoadingScene::loadLogInInformation(){
    
    MainUser->UserNameStr = CCUserDefault::sharedUserDefault()->getStringForKey("name_", "");
    MainUser->PassWordStr = CCUserDefault::sharedUserDefault()->getStringForKey("psw_", "");
    cout<<"上次记录的账号为"<<MainUser->UserNameStr<<endl;
    cout<<"上次记录的账号为"<<MainUser->PassWordStr<<endl;
    //MainUser->UserNameStr = "shamoz";
    //MainUser->PassWordStr = "229163lxydbc";
    
    MainUser->introduced = CCUserDefault::sharedUserDefault()->getBoolForKey("Introduced", false);
    
    //1.都不为空
    if (MainUser->UserNameStr!="" && MainUser->PassWordStr!="") {   return 1;    }
    //2.都为空
    else if(MainUser->UserNameStr == "" && MainUser->PassWordStr == ""){   return 0;   }
    //3.有一个为空
    else{    return 2;   }
}

void LoadingScene::TimeCounter(){
    if (counter < 180)
        counter++;
    else{
        unschedule(schedule_selector(LoadingScene::TimeCounter));
        MainUser->hasLogIn = false;
        CCMessageBox("网络超时,请检查您的网络", "温馨提示");
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, LogInScene::scene()));
    }
}