#include "LoadingScene.h"
#include "global.h"
#include "ASUser.h"
#include <sys/timeb.h>
using namespace std;

ASUser* MainUser;

CCScene* LoadingScene::scene(){
    CCScene *scene = CCScene::create();
    LoadingScene *layer = LoadingScene::create();
    scene->addChild(layer);
    return scene;
}

bool LoadingScene::init(){
    if ( !CCLayer::init() ){return false;}
    
    CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("0.GeneralPart.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("1.LoginScene.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("4.TibaoSelection.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("5.RankScene.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("MikuFight.plist");
    
    MainUser = ASUser::createUser();
    LoadingSceneUI();

    int index = loadLogInInformation();
    if (index == 1 || index == 2)
        LogInHTTP();
    else if(index == 0)
        FastSignUp();
    
    schedule(schedule_selector(LoadingScene::TimeCounter),1);

    return true;
}

void LoadingScene::LoadingSceneUI(){
    
    struct timeb tp;
    ftime(&tp);
    std::srand(tp.millitm);
    
    int index = rand()%20;
    string tipStr[20] = {
        "阿葱音的“余音”和回复道具都能使生命超过上限",
        "钢蛋侠的“干扰”和乌云道具无法叠加",
        "呜喵王的“折磨”造成的额外伤害无法暴击",
        "剑小圣的“嗜血”的反噬效果无法让自己致死",
        "谢耳朵是基础攻击和成长最高的角色之一",
        "初始状态下发动攻击至少需要5点行动力",
        "据说每额外多积累1点行动力，暴击几率就会增加哦",
        "在你生命较低时，对阿葱音造成暴击是件可怕的事",
        "暴击药水绝对是剑小圣的必备道具之一",
        "不要光顾着答题哦，想办法击败对手才能获取胜利！",
        "造成暴击时的攻击特效比普通攻击特效更加酷炫拽",
        "每次答错题目都会受到最大生命20%的伤害，要小心呀",
        "多关注双方头像区域的信息才能够知己知彼，百战百胜！",
        "每个角色10级后，都能够通过升级解锁各自的天赋能力",
        "谢耳朵的“无语”搭配乌云道具会让对手痛不欲生",
        "排名信息和战斗统计是按照每个题包单独计算的哦",
        "每周参加一个题包的比赛不足十场时,\n积分不会计入天梯系统",
        "击败高排位等级的玩家会获得更多金币",
        "每局比赛最多可以携带三种道具",
        "赛前购买的道具只在一局比赛中有效，不要浪费啦"
    };
    
    //1.Tips
    tips = CCLabelTTF::create(tipStr[index].c_str(), "Arial Rounded MT bold", 25);
    tips->setPosition(ccp(size.width/2,size.height*9.3/20));
    addChild(tips,0);
    
    //2.提示框上的小脑袋
    int headIndex = rand()%6+1;
    string tex = "head_" + int2string(headIndex) + ".png";
    for (int i = 0;  i < 3 ; i ++) {
        littleHead[i] = CCSprite::createWithSpriteFrameName(tex.c_str());
        littleHead[i]->setOpacity(0);
        littleHead[i]->setPosition(ccp(size.width/2-size.width/14+size.width*i/15, size.height*11/20));
        addChild(littleHead[i],0);
    }
    
    //3.提示框上的人物
    string heroTex[6] = {"BladeMaster.png","IronMan.png","Miku.png","Sheldon.png","LichKing.png","Luffy.png"};
    CCSprite* hero = CCSprite::createWithSpriteFrameName(heroTex[headIndex-1].c_str());
    hero->setScale(0.5);
    hero->setPosition(ccp(size.width*2/7, size.height*4/7));
    //addChild(hero,2);
    
    //4.小脑袋一直动
    schedule(schedule_selector(LoadingScene::RollHead),1);
}

void LoadingScene::RollHead(){
    
    if (headCounter < 3){
        littleHead[headCounter]->setOpacity(255);
        headCounter++;
    }
    else{
        headCounter = 0;
        littleHead[0]->setOpacity(0);
        littleHead[1]->setOpacity(0);
        littleHead[2]->setOpacity(0);
    }
}
