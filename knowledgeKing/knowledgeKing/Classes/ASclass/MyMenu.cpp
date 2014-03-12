#include "MyMenu.h"
using namespace cocos2d;

MyMenu* MyMenu::create()
{
    return MyMenu::create(NULL, NULL);
}

MyMenu * MyMenu::create(CCMenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    MyMenu *pRet = MyMenu::createWithItems(item, args);
    
    va_end(args);
    
    return pRet;
}

MyMenu* MyMenu::createWithArray(CCArray* pArrayOfItems)
{
    MyMenu *pRet = new MyMenu();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

MyMenu* MyMenu::createWithItems(CCMenuItem* item, va_list args)
{
    
    CCArray* pArray = NULL;
    if( item )
    {
        pArray = CCArray::create(item, NULL);
        CCMenuItem *i = va_arg(args, CCMenuItem*);
        while(i)
        {
            pArray->addObject(i);
            i = va_arg(args, CCMenuItem*);
        }
    }
    
    return MyMenu::createWithArray(pArray);
}




void MyMenu::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}


bool MyMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    moved=false;
    return CCMenu::ccTouchBegan(pTouch,pEvent);
}

void MyMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    moved=true;
    CCMenu::ccTouchMoved(pTouch,pEvent);
}

void MyMenu::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!moved)
        CCMenu::ccTouchEnded(pTouch,pEvent);
    else
        m_eState = kCCMenuStateWaiting;
}

