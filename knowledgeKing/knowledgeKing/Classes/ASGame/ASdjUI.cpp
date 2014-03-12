#include "ASGame.h"
#include "ASBot.h"
#include "global.h"

extern ASBot* bot2;

void ASGame::ASdjUI(){
    
    CCPoint pos[3] = {ccp(size.width*510/1500,size.height*8.5/150),ccp(size.width*755/1500, size.height*8.5/150),ccp(size.width*1000/1500, size.height*8.5/150)};
    CCMenu* menu = CCMenu::create();
    for (int i = 0 ; i < 3 ; i++) {
        
        if (dj_tag[i]!=-1) {

            string item = "daoju_" + int2string(dj_tag[i]) + ".png";
            string item_s = "daoju_" + int2string(dj_tag[i]) + "-s.png";
            CCSprite* itemSprite = CCSprite::createWithSpriteFrameName(item.c_str());
            CCSprite* itemSprite_s = CCSprite::createWithSpriteFrameName(item_s.c_str());
            CCMenuItemSprite* djItem = CCMenuItemSprite::create(itemSprite,itemSprite_s,this,menu_selector(ASGame::djIsSelected));
            djItem->setScale(1);
            djItem->setPosition(pos[i]);
            djItem->setTag(playerNumber*10000000+77777+i);
            menu->addChild(djItem,playerNumber*10000000+77777+i);
            
            CCLabelTTF* number = CCLabelTTF::create(int2string(dj_number[i]).c_str(),"Arial Rounded MT bold",36);
            number->setColor(ccc3(0,0,0));
            number->setPosition(ccp(pos[i].x + size.width*75/1500 , pos[i].y - size.height*50/1500));
            addChild(number,4,playerNumber*10000000+87777+i);
        }
    }
    menu->setPosition(CCPointZero);
    addChild(menu,3,playerNumber*10000000+77776);
}

void ASGame::djIsSelected(CCObject* sender){
    
    int tmp = ((CCMenuItemSprite*)sender)->getTag() - (playerNumber*10000000+77777);  //第几个按钮
    int djTag = dj_tag[tmp]; //这个按钮是什么道具
    
    if (dj_number[tmp]>0) {
        //1.乌云
        if (djTag == 1 && !bot2->clouded) {
            coverQuestion(15);
            changeDaoJuNumber(tmp);
        }
        //2.吹风机
        else if(djTag == 2 && clouded){
            blowCloudAway();
            changeDaoJuNumber(tmp);
        }
        //3.跳过本题
        else if(djTag == 3){
            skipThisQuestion();
            changeDaoJuNumber(tmp);
        }
        //4.加血
        else if(djTag == 4){
            addShengMing();
            changeDaoJuNumber(tmp);
        }
        //5.加行动力
        else if(djTag == 5){
            addMoveCount();
            changeDaoJuNumber(tmp);
        }
        //6.加爆击
        else if(djTag == 6){
            addCriticalRate();
            changeDaoJuNumber(tmp);
        }
    }

    if (dj_number[tmp] == 0) {   //用完以后,设置为空
        ((CCMenuItemFont*)sender)->setEnabled(false);
        removeChildByTag(playerNumber*10000000+77777+tmp);
        removeChildByTag(playerNumber*10000000+87777+tmp);
    }
}

void ASGame::changeDaoJuNumber(int _tag){
    dj_number[_tag]--;
    CCLabelTTF* number = (CCLabelTTF*)this->getChildByTag(playerNumber*10000000+87777+_tag);
    number->setString(int2string(dj_number[_tag]).c_str());
}

