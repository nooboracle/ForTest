#ifndef __knowledgeKing__ASUser__
#define __knowledgeKing__ASUser__
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;

class ASUser: CCObject{
public:
    
    bool introduced = false;
    bool hasChosenHero = false;
    bool hasLogIn = false;
    bool muted = false;
    
    string UserID;
    string UserNameStr;
    string PassWordStr;
    string UserTokenStr;
    string gender;
    string nickName;
    int firstHero;
    int HeroSelected;
    int gold = 0;
    int energyNumber;
    int nextEnergyRecoveryCounter;
    string HeroLevelStr[10] = {};
    int HeroLevel[10] = {1,1,1,1,1,1,1,1,1,1};
    int HeroLevelUpGold[100] = {100,200,300,400,500,600,700,800,3400,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000};
    bool HeroUnLocked[10] = {true,false,false,false,false,false,false,false,false,false};
    int HeroUnLockPrice[10] = {10000,10000,10000,15000,20000,10000,10000,10000,10000,10000};
    
    //预加载
    string TextureTxTList[100];
    string TexturePlist[100];
    int TextureType[1000];
    int needToPreload[1000];
    string TextureFileName[1000];
    
    //Rank
    int Rank[16];
    int WinTimes[16];
    int LoseTimes[16];
    float WinPercent[16];
    int calculateRank(int _id,int _winTimes,int _loseTimes);
    int RankListCounter = 0;
    
    static ASUser* createUser();
    virtual bool initUser(std::vector<char> *buffer);
    void getHeroInformationHTTP();
    void getHeroInformationRequestCompleted(CCNode* sender,void* data);
    void printJson(std::vector<char> *buffer);
    
};
#endif /* defined(__knowledgeKing__ASUser__) */


