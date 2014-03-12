#ifndef knowledgeKing_AsChooseHero_h
#define knowledgeKing_AsChooseHero_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "AsMessageBox.h"
#include "MyMenu.h"
using namespace cocos2d;
using namespace extension;
using namespace std;

class AsChooseHero:public CCLayer,public CCEditBoxDelegate,public CCScrollViewDelegate{
public:
    
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    //1.适配
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    //2.信息
    string nickName = "";
    string gender = "male";
    int FirstHeroSelected = -1;
    AsMessageBox* box;
    CCLayer* ContainerLayer;
    CCSprite* greenMark;
    CCSprite* HeroSelectedMark;
    MyMenu* heroMenu;
    
    //3.UI
    void chooseHeroUI();
    void randomNickName();
    void selectGender(CCNode* sender);
    void selectFirstHero(CCNode* sender);
    void goToTiBaoSelection();

    //4.HTTP
    void updateGenderAndNickHTTP();
    void updateGenderAndNickRequestCompleted(CCNode* sender, void* data);
    void printJson(std::vector<char> *buffer);
    void unlockHeroHTTP(int _HeroID);
    void unlockHeroRequestCompleted(cocos2d::CCNode *sender, void *data);
    void getRandomNameHTTP();
    void getRandomNameRequestCompleted(CCNode* sender, void* data);
    
    CREATE_FUNC(AsChooseHero);
    
    virtual void editBoxEditingDidBegin(CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(CCEditBox *editBox);
    virtual void editBoxTextChanged(CCEditBox *editBox,const std::string &text);
    virtual void editBoxReturn(CCEditBox *editBox);
        
    void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);
    virtual bool ccTouchBegan(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* ptouch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();

};


#endif
