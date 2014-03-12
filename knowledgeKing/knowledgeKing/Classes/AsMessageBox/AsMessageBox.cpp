#include "AsMessageBox.h"
#include "SimpleAudioEngine.h"
#include "ASUser.h"
#include "RankScene.h"
using namespace CocosDenshion;
extern ASUser* MainUser;

AsMessageBox* AsMessageBox::createMessageBox(string _message,int _type,int _subType){
    AsMessageBox* AsMessageBoxLayer = new AsMessageBox();
    if (AsMessageBoxLayer){
        
        AsMessageBoxLayer->type = _type;
        AsMessageBoxLayer->subType = _subType;
        
        if (_type == 1)
            AsMessageBoxLayer->initLoadingBox(_message);
        else if(_type == 2)
            AsMessageBoxLayer->initMessageBox(_message,_subType);
        else if(_type == 5)
            AsMessageBoxLayer->initFightInfo(_message);
        else if(_type == 3)
            AsMessageBoxLayer->initShareMessageBox(_subType);
        return AsMessageBoxLayer;
    }
    return NULL;
}

void AsMessageBox::closeMessageBox(){
    if (type == 1)
        unscheduleAllSelectors();
    this->removeFromParentAndCleanup(true);
}

void AsMessageBox::removeThis(CCNode* sender){removeChild(sender,true);}
void AsMessageBox::changeMessage(string _message){message->setString(_message.c_str());}

void AsMessageBox::onEnter(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);
    CCLayer::onEnter();
}
void AsMessageBox::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}
bool AsMessageBox::ccTouchBegan(CCTouch* ptouch,CCEvent* event){
    if ( (type == 2 && (subType == 1 || subType == 2 || subType == 4)) || type == 3  || type == 6 ) {
        m_bTouchedMenu = closeMenu->ccTouchBegan(ptouch, event);
    }
    return true;
}
void AsMessageBox::ccTouchMoved(CCTouch* ptouch, CCEvent* event){
    if ( (type == 2 && (subType == 1 || subType == 2 || subType == 4)) || type == 3  || type == 6 ) {
        if (m_bTouchedMenu)
            closeMenu->ccTouchMoved(ptouch, event);
    }
}
void AsMessageBox::ccTouchEnded(CCTouch* ptouch, CCEvent* event){
    if ( (type == 2 && (subType == 1 || subType == 2 || subType == 4)) || type == 3  || type == 6 ) {
        if (m_bTouchedMenu) {
            closeMenu->ccTouchEnded(ptouch, event);
            m_bTouchedMenu = false;
        }
    }
}
void AsMessageBox::ccTouchCancelled(CCTouch* ptouch, CCEvent* event){
    if ( (type == 2 && (subType == 1 || subType == 2 || subType == 4)) || type == 3  || type == 6 ) {
        if (m_bTouchedMenu) {
            closeMenu->ccTouchEnded(ptouch, event);
            m_bTouchedMenu = false;
        }
    }
}


