#include "LogInScene.h"

void LogInScene::LogInTableDraw(int _index){
    
    CCMenu* popUpMenu = (CCMenu*)popUpMenuLayer->getChildByTag(998);
    CCMenu* beginMenu = (CCMenu*)this->getChildByTag(997);
    popUpMenu->setTouchEnabled(false);
    if (beginMenu!=NULL)
        beginMenu->setTouchEnabled(false);
    
    //1.基本层
    AccountLayer = CCLayer::create();
    AccountLayer->setPosition(CCPointZero);
    addChild(AccountLayer,10);
    
    //2.黑色遮罩:不在该移动层上
    CCLayerColor* black = CCLayerColor::create(ccc4(0, 0, 0, 155));
    black->setPosition(CCPointZero);
    AccountLayer->addChild(black,0);
    
    //3.木板
    CCSprite* LogInTable = CCSprite::createWithSpriteFrameName("LogInTable.png");
    LogInTable->setPosition(ccp(size.width/2, size.height/2+winDif*2));
    AccountLayer->addChild(LogInTable,1);
    
    //3.edit box - user name
    CCScale9Sprite *UserNameTexture = CCScale9Sprite::createWithSpriteFrameName("textTable.png");
    CCEditBox *UserNameBox=CCEditBox::create(CCSizeMake(195, 44), UserNameTexture);
    UserNameBox->setPosition(ccp(size.width*8/15, size.height*0.561+winDif*2));
    UserNameBox->setFontName("Arial Rounded MT bold");
    UserNameBox->setMaxLength(20);
    UserNameBox->setInputMode(kEditBoxInputModeAny);
    UserNameBox->setInputFlag(kEditBoxInputFlagSensitive);
    UserNameBox->setReturnType(kKeyboardReturnTypeDone);
    UserNameBox->setDelegate(this);
    AccountLayer->addChild(UserNameBox,2,10);
    
    //4.edit box - password
    CCScale9Sprite *PasswordTexture = CCScale9Sprite::createWithSpriteFrameName("textTable.png");
    CCEditBox *PasswordBox = CCEditBox::create(CCSizeMake(195, 44), PasswordTexture);
    PasswordBox->setPosition(ccp(size.width*8/15, size.height*0.486+winDif*2));
    PasswordBox->setFontName("Arial Rounded MT bold");
    PasswordBox->setMaxLength(50);
    PasswordBox->setInputMode(kEditBoxInputModeAny);
    PasswordBox->setInputFlag(kEditBoxInputFlagPassword);
    PasswordBox->setReturnType(kKeyboardReturnTypeDone);
    PasswordBox->setDelegate(this);
    AccountLayer->addChild(PasswordBox,2,11);
    
    //5.登录注册按钮
    CCMenuItemSprite* Item1;
    if (_index == 0){
        CCSprite* item1Sprite = CCSprite::createWithSpriteFrameName("signUpButtonGreen.png");
        CCSprite* item1Sprite_s = CCSprite::createWithSpriteFrameName("signUpButtonGreen-s.png");
        Item1 = CCMenuItemSprite::create(item1Sprite,item1Sprite_s,this,menu_selector(LogInScene::SignUpHTTP));
    }
    else if (_index == 1){
        CCSprite* item1Sprite = CCSprite::createWithSpriteFrameName("binding.png");
        CCSprite* item1Sprite_s = CCSprite::createWithSpriteFrameName("binding-s.png");
        Item1 = CCMenuItemSprite::create(item1Sprite,item1Sprite_s,this,menu_selector(LogInScene::bindUserAccountHTTP));
    }
    else if (_index == 2){
        CCSprite* item1Sprite = CCSprite::createWithSpriteFrameName("LogInButton.png");
        CCSprite* item1Sprite_s = CCSprite::createWithSpriteFrameName("LogInButton-s.png");
        Item1 = CCMenuItemSprite::create(item1Sprite,item1Sprite_s,this,menu_selector(LogInScene::LogInHTTP));
    }
    CCSprite* item2Sprite = CCSprite::createWithSpriteFrameName("cancelButton.png");
    CCSprite* item2Sprite_s = CCSprite::createWithSpriteFrameName("cancelButton-s.png");
    CCMenuItemSprite* Item2 = CCMenuItemSprite::create(item2Sprite,item2Sprite_s,this,menu_selector(LogInScene::closeThisLayer));
    Item1->setPosition(ccp(winSize.width*11.5/30,size.height*0.42+winDif*2));
    Item2->setPosition(ccp(winSize.width*18.5/30,size.height*0.42+winDif*2));
    CCMenu* LogInMenu = CCMenu::create(Item1,Item2,NULL);
    LogInMenu->setPosition(CCPointZero);
    AccountLayer->addChild(LogInMenu,3);
}

void LogInScene::closeLogInTable(){
    CCMenu* popUpMenu = (CCMenu*)popUpMenuLayer->getChildByTag(998);
    CCMenu* beginMenu = (CCMenu*)this->getChildByTag(997);
    if(beginMenu!=NULL)
        beginMenu->setTouchEnabled(true);
    popUpMenu->setTouchEnabled(true);

    removeChild(AccountLayer);
}

void LogInScene::editBoxEditingDidBegin(CCEditBox *editBox){}
void LogInScene::editBoxEditingDidEnd(CCEditBox *editBox){
    if (editBox->getTag() == 10) {  UserNameStr = editBox->getText();   }
    else if(editBox->getTag() == 11){   PassWordStr = editBox->getText();   }
}
void LogInScene::editBoxTextChanged(CCEditBox *editBox,const std::string &text){}
void LogInScene::editBoxReturn(CCEditBox *editBox){}

void LogInScene::removeThis(CCNode* sender){    removeChild(sender,true);    }