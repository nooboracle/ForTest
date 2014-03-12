#include "cocos2d.h"
#include "cocos-ext.h"
#include "AsMessageBox.h"
#include "MyMenu.h"
using namespace std;
using namespace cocos2d;
using namespace extension;

class RankScene: public CCLayer, public CCScrollViewDelegate{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    //1.UI部分
    CCScrollView* roleScrollView;
    CCLayerColor* ContainerLayer;
    CCLayer* layerToRoll;
    CCLayerColor* matchLayer;
    CCLayerColor* LevelUpHeroDetailLayer;
    CCLayer* roleScrollLayer;
    CCLabelTTF* daysLeftLabel;
    AsMessageBox* MessageBox;
    string energyStr;
    string year_current,year_next,month_current,month_next,day_current,day_next,clock_current,clock_next,minute_current,minute_next,second_current,second_next;
    int nextRecoverySecond = 0;

    CCMenu* bottomMenu;
    CCMenu* backMenu;
    MyMenu* buyMenu;
    MyMenu* chooseMenu;
    CCMenu* beginMenu;
    CCMenu* moveMenu;
    CCMenu* LevelUpMenu;
    CCMenu* LevelUpMenuDetail;
    int tableIndex;  //0 ranklist 1 ranklistlist 2 daojulist
    int serverTime;  //还差多久时间结算
    bool lefting = false;
    bool righting = false;
    bool matched = false;
    int roleIndex = 1;
    bool LevelUpLayerHasBeenOpened = false;
    int HeroProcessing;
    int goldProcessingIndex;  //1是购买体力 2是匹配前的结算
    
    //2.排序的内容
    string nickName_Rank[20];
    string winPercent_Rank[20];
    string weeklyWin_Rank[20];
    string userImage[20] = {"userImage_1.png","userImage_2.png","userImage_3.png","userImage_4.png","userImage_1.png","userImage_2.png","userImage_3.png","userImage_4.png","userImage_1.png","userImage_2.png","userImage_3.png","userImage_4.png","userImage_1.png","userImage_2.png","userImage_3.png","userImage_4.png","userImage_1.png","userImage_2.png","userImage_3.png","userImage_4.png"};
    
    //3.排行榜UI
    void BasicUIDraw();
    void rankListListDraw();
    void rankListDraw(int _tibaoType);
    void daojuListDraw();
    void disableButtons();
    void enableButtons();
    void backToTibaoSelection();
    void inviteFriends();
    void shareRank();
    
    //4.三个排行榜的内容
    void rankListContent();
    void ListListContent();
    void DaoJuContent();
    
    //5.选择英雄
    void pickListDraw();
    void BeginGame(CCNode* sender);
    void changeToBeginButton(CCNode* sender);
    void changeToModeButton(CCNode* sender);
    void moveLeft();
    void moveRight();
    void resetLeftBool();
    void resetRightBool();
    
    //6.升级英雄
    void openLevelUpHeroLayer(CCNode* sender); //其中也包括解锁
    void LevelUpAnimation(int _index);
    void UnlockAnimation(int _index);
    void LevelUpHeroDetail();
    void closeLevelUpHeroDetail();
    void LevelUpHero();
    
    //7.金币
    void changeGold(int _gold);
    void changeGoldHTTP();
    void changeGoldRequestCompleted(CCNode* sender,void* data);
    
    //8.三个板子之间切换
    void djToRankList();
    void goToRankListList();
    void goToDaojuList();
    void goToRankList();
    void stableRankList();
    void stableListList();
    void stableDaoJuList();
    void resetToListList();
    void resetToDaoJuList();
    void resetToRankList();
    void middleProcessToListList();
    void middleProcessToRankList();
    void middleProcessToDaoJuList();
    void middleProcessFromDjToRankList();
    
    void tibaoRankIsSelected(CCNode* sender);
    
    //9.选择道具
    void chooseDj(CCNode* sender);

    //10.商店页面
    void showShopLayer();
    void buyGold(CCObject* sender);
    
    //11.http
    //11.1 排行榜
    void getRankListHTTP();
    void getRankListRequestCompleted(CCNode* sender,void* data);
    void getRankHTTP();
    void getRankRequestCompleted(CCNode* sender,void* data);
    void getServerTime();
    void getServerTimeRequestCompleted(CCNode* sender,void* data);
    void HeroLevelUpHTTP(int _HeroIndex,int _HeroLevel);
    void HeroLevelUpRequestCompleted(CCNode* sender,void* data);
    void unlockHeroHTTP(int _HeroIndex);
    void unlockHeroRequestCompleted(CCNode* sender,void* data);
    void getEnergyHTTP();
    void getEnergyRequestCompleted(CCNode* sender,void* data);
    void MinusEnergyHTTP();
    void MinusEnergyRequestCompleted(CCNode* sender,void* data);
    void printJson(std::vector<char> *buffer);
    void updateUserGold();
    void updateUserGoldRequestCompleted(CCNode* sender,void* data);
    void updateUserEnergy();
    void updateUserEnergyRequestCompleted(CCNode* sender,void* data);
    void getRandomName();
    void getRandomNameRequestCompleted(CCNode* sender,void* data);
    
    //11.2获取机器人信息
    void getRobotInformation();
    void getRobotInformationCompleted(CCNode* sender, void* data);
    
    //12.匹配对手
    void openMatchLayer();
    void closeMatchLayer();
    void setHeroCard();
    void BotMatched();
    void BeginOnLineGame();
    void removeThis(CCNode* sender);
    
    //13. 体力回复时间
    void energyTimeCounter();
    void buyEnergy();
    
    CREATE_FUNC(RankScene);
    
    void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);
};


