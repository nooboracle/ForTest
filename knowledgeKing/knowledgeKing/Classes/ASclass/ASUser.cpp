#include "ASUser.h"
#include "global.h"

 ASUser* ASUser::createUser(){
    
    ASUser* user = new ASUser();
    if (user) {     return user;    }
    else{
        CC_SAFE_DELETE(user);
        return NULL;
    }
}

bool ASUser::initUser(std::vector<char> *buffer){
    UserID = "";
    UserNameStr = "";
    UserTokenStr = "";
    gender = "";
    nickName = "";
    for (int i = 0 ; i < 5; i++)
        HeroLevelStr[i] = "";
    
    string goldStr;
    for (unsigned int i = 0; i < buffer->size(); i++) {
        //0.ID
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'i' && (*buffer)[i+2] == 'd' && (*buffer)[i+3] == '"' && (*buffer)[i+4] == ':') {
            for (int j = i + 5; (*buffer)[j] != ','; j++) {   UserID += (*buffer)[j];    }
        }
        //1.user name
        else if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'n' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'm' && (*buffer)[i+4] == 'e') {
            for (int j = i + 8; (*buffer)[j] != '"'; j++) {   UserNameStr += (*buffer)[j];    }
        }
        //2.token
        else if ((*buffer)[i] == '"' && (*buffer)[i+1] == 't' && (*buffer)[i+2] == 'o' && (*buffer)[i+3] == 'k' && (*buffer)[i+4] == 'e' && (*buffer)[i+5] == 'n' && (*buffer)[i+6] == '"') {
            for(int j = i + 9 ; (*buffer)[j] != '"' ; j++){   UserTokenStr += (*buffer)[j];   }
        }
        //3.gold
        else if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'g' && (*buffer)[i+2] == 'o' && (*buffer)[i+3] == 'l' && (*buffer)[i+4] == 'd') {
            for(int j = i + 7 ; (*buffer)[j] != ',' ; j++){     goldStr += (*buffer)[j];    }
            gold = atoi(goldStr.c_str());
        }
        //5.gender
        else if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'g' && (*buffer)[i+2] == 'e' && (*buffer)[i+3] == 'n' && (*buffer)[i+4] == 'd' && (*buffer)[i+5] == 'e' && (*buffer)[i+6] == 'r') {
            for(int j = i + 10 ; (*buffer)[j] != '"' ; j++){     gender += (*buffer)[j];    }
        }
        //6.nick name
        else if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'n' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 'c' && (*buffer)[i+4] == 'k') {
            for(int j = i + 8 ; (*buffer)[j] != '"' ; j++){     nickName += (*buffer)[j];    }
        }
        //7.密码
        else if((*buffer)[i] == '"' && (*buffer)[i+1] == 'p' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 's' && (*buffer)[i+4] == 's' && (*buffer)[i+5] == 'w' && (*buffer)[i+6] == 'o') {
            for(int j = i + 12 ; (*buffer)[j] != '"' ; j++){   PassWordStr += (*buffer)[j];   }
        }
    }
    if (nickName == "ull,")
        nickName = UserNameStr;
    
    //8.是否已经进行过教学
    introduced = CCUserDefault::sharedUserDefault()->getBoolForKey("Introduced",false);
    
    cout<<"登录信息为:"<<endl;
    cout<<"1.id:"<<UserID<<endl;
    cout<<"2.用户名:"<<UserNameStr<<endl;
    cout<<"3.密码:"<<PassWordStr<<endl;
    cout<<"4.token:"<<UserTokenStr<<endl;
    cout<<"5.金币数:"<<gold<<endl;
    cout<<"6.性别:"<<gender<<endl;
    cout<<"7.昵称:"<<nickName<<endl;
    cout<<"8.是否已经进行过教学:"<<introduced<<endl;
    
    getHeroInformationHTTP();
    
    if (gender == "" || nickName == "") {     return false;       }
    return true;
}