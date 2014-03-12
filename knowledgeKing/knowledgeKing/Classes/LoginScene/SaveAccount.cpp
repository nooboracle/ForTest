#include "LogInScene.h"
#include "ASUser.h"
using namespace std;

extern ASUser* MainUser;

void LogInScene::saveLogInInformation(string _name,string _password){
    
    cout<<_name<<" "<<_password<<endl;
    CCUserDefault::sharedUserDefault()->setStringForKey("name_", _name);
    CCUserDefault::sharedUserDefault()->setStringForKey("psw_", _password);
}
