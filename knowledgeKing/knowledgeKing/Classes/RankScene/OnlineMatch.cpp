#include "RankScene.h"
#include "ASUser.h"
#include "global.h"
#include "GameBaseClass.h"
#include "SimpleAudioEngine.h"
#include <sys/timeb.h>
using namespace CocosDenshion;
using namespace std;

extern ASUser* MainUser;
string tmpBotID;
string tmpBotName;
string tmpBotToken;
string tmpBotNickName;
string  tmpBotRankStr;
string tmpBotBaseRankScoreStr;
string tmpBotHeroIDStr;
string tmpBotHeroLevelStr;
string tmpBotGoldStr;
string tmpBotAllHeroLevelStr[50];
int tmpBotAnswerLV;
int tmpBotItemLV;
int tmpBotHeroID;
int tmpBotHeroLevel;
int tmpBotRank;
int tmpBotBaseRankScore;
int tmpBotGold;
int tmpBotAllHeroLevel[50];
extern string tibaoName[100];
extern int tibaoType;

void RankScene::openMatchLayer(){
    
    if(!MainUser->muted){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("123.mp3", true);
    }
    
    disableButtons();
    
    struct timeb tp;
    ftime(&tp);
    std::srand(tp.millitm);
    
    getRobotInformation();
    
    //1.黑色底层
    matchLayer = CCLayerColor::create(ccc4(0,0,0,155));
    matchLayer->setPosition(CCPointZero);
    addChild(matchLayer,10001,997);
    
    //2.匹配框
    CCSprite* matchTable = CCSprite::createWithSpriteFrameName("matchTable.png");
    matchTable->setPosition(ccp(size.width/2,size.height/2+winDif*2*alpha));
    matchLayer->addChild(matchTable,101);
    
    //3.匹配中的字
    CCLabelTTF* matching = CCLabelTTF::create("匹配对手中，请稍后","Arial Rounded MT bold",36);
    matching->setColor(ccc3(30,102,129));
    matching->setPosition(ccp(size.width/2,size.height*3.2/10+winDif*2*alpha));
    matchLayer->addChild(matching,102,994);
    
    //4.两个人物
    string MainUserCardTex = "card_" + int2string(MainUser->HeroSelected) + ".png";
    CCSprite* MainUserCard = CCSprite::createWithSpriteFrameName(MainUserCardTex.c_str());
    MainUserCard->setPosition(ccp(size.width*2/7,size.height*57/100+winDif*2*alpha));
    matchLayer->addChild(MainUserCard,102);
    
    CCSprite* BotCard = CCSprite::createWithSpriteFrameName("card_2.png");
    BotCard->setPosition(ccp(size.width*5/7, size.height*57/100+winDif*2*alpha));
    matchLayer->addChild(BotCard,102,996);
    
    //5.角色名字
    CCLabelTTF* MainUserName = CCLabelTTF::create(MainUser->nickName.c_str(), "Arial Rounded MT bold", 30);
    MainUserName->setColor(ccc3(30,102,129));
    MainUserName->setPosition(ccp(size.width*2/7, size.height*38/100+winDif*2*alpha));
    matchLayer->addChild(MainUserName,102);
    
    string randomXing[1000] = {
        "安陵","安平","安期","安阳","白马","百里","柏侯","鲍俎","北宫","北郭","北门","北山",
        "北唐","奔水","逼阳","宾牟","薄奚","薄野","曹牟","曹丘","常涛","长鱼","车非","成功",
        "成阳","乘马","叱卢","丑门","樗里","淳子","穿封","答禄","达勃","达步","达奚","淡台",
        "邓陵","第五","地连","地伦","东方","东里","东南","东宫","东门","东乡","东丹","东郭",
        "东陵","东关","东闾","东阳","东野","东莱","豆卢","斗于","都尉","独孤","端木","段干",
        "多子","尔朱","方雷","丰将","封人","封父","夫蒙","夫馀","浮丘","傅余","干已","高车",
        "高陵","高堂","高阳","高辛","皋落","哥舒","盖楼","庚桑","梗阳","宫孙","公羊","公良",
        "公孙","公罔","公西","公冶","公敛","公梁","公输","公上","公山","公户","公玉","公仪",
        "公仲","公坚","公伯","公祖","公乘","公晰","公族","姑布","古口","古龙","古孙","谷梁",
        "谷浑","瓜田","关龙","鲑阳","归海","函治","韩馀","罕井","浩生","浩星","纥骨","纥奚","纥于","贺拨","贺兰","贺楼","赫连","黑齿","黑肱","侯冈","呼延","壶丘","呼衍","斛律","胡非","胡母","胡毋","皇甫","皇父","兀官","吉白","即墨","季瓜","季连","季孙","茄众","蒋丘","金齿","晋楚","京城","泾阳","九百","九方","睢鸠","沮渠","巨母","勘阻","渴侯","渴单","可汗","空桐","空相","昆吾","老阳","乐羊","荔菲","栎阳","梁丘","梁由","梁馀","梁垣","陵阳","伶舟","冷沦","令狐","刘王","柳下","龙丘","卢妃","卢蒲","鲁步","陆费","角里","闾丘","马矢","麦丘","茅夷","弥牟","密革","密茅","墨夷","墨台","万俊","昌顿","慕容","木门","木易","南宫","南郭","南门","南荣","欧侯","欧阳","逄门"};
    
    string randomMing[1000] = {"含蕊","亦玉","靖荷","碧萱","寒云","向南","书雁","怀薇","思菱","忆文","翠巧","怀山","若山","向秋","凡白","绮烟","从蕾","天曼","又亦","依琴","曼彤","沛槐","又槐","元绿","安珊","夏之","易槐","宛亦","白翠","丹云","问寒","易文","傲易","青旋","思真","妙之","半双","若翠","初兰","怀曼","惜萍","初之","宛丝","寄南","小萍","幻儿","千风","天蓉","雅青","寄文","代天","春海","惜珊","向薇","冬灵","惜芹","凌青","谷芹","香巧","雁桃","映雁","书兰","盼香","向山","寄风","访烟","绮晴","傲柔","寄容","以珊","紫雪","芷容","书琴","寻桃","涵阳","怀寒","易云","采蓝","代秋","惜梦","尔烟","谷槐","怀莲","涵菱","水蓝","访冬","半兰","又柔","冬卉","安双"};
    string randomedName = randomXing[rand()%200] + randomMing[rand()%80];
    CCLabelTTF* BotName = CCLabelTTF::create(randomedName.c_str(), "Arial Rounded MT bold", 30);
    BotName->setColor(ccc3(30,102,129));
    BotName->setPosition(ccp(size.width*5/7, size.height*38/100+winDif*2*alpha));
    matchLayer->addChild(BotName,102,995);
    
    //6.随机卡牌序号
    schedule(schedule_selector(RankScene::setHeroCard),0.2);
    
    //7.题包名称
    CCLabelBMFont* name = CCLabelBMFont::create(tibaoName[tibaoType].c_str(), "puzzlename_list2.fnt");
    name->setScale(0.417);
    name->setPosition(ccp(size.width/2,size.height*5.35/7+winDif*2*alpha));
    matchLayer->addChild(name,102);
    
    CCActionInterval* fadeIn = CCFadeIn::create(0.3);
    matchLayer->runAction(fadeIn);
}

void RankScene::setHeroCard(){
    int _index = rand()%5 + 1;
    string texName = "card_" + int2string(_index) + ".png";
    CCSprite* BotCard = (CCSprite*)matchLayer->getChildByTag(996);
    BotCard->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str()));
    
    string randomXing[1000] = {"安陵","安平","安期","安阳","白马","百里","柏侯","鲍俎","北宫","北郭","北门","北山","北唐","奔水","逼阳","宾牟","薄奚","薄野","曹牟","曹丘","常涛","长鱼","车非","成功","成阳","乘马","叱卢","丑门","樗里","淳子","穿封","答禄","达勃","达步","达奚","淡台","邓陵","第五","地连","地伦","东方","东里","东南","东宫","东门","东乡","东丹","东郭","东陵","东关","东闾","东阳","东野","东莱","豆卢","斗于","都尉","独孤","端木","段干","多子","尔朱","方雷","丰将","封人","封父","夫蒙","夫馀","浮丘","傅余","干已","高车","高陵","高堂","高阳","高辛","皋落","哥舒","盖楼","庚桑","梗阳","宫孙","公羊","公良","公孙","公罔","公西","公冶","公敛","公梁","公输","公上","公山","公户","公玉","公仪","公仲","公坚","公伯","公祖","公乘","公晰","公族","姑布","古口","古龙","古孙","谷梁","谷浑","瓜田","关龙","鲑阳","归海","函治","韩馀","罕井","浩生","浩星","纥骨","纥奚","纥于","贺拨","贺兰","贺楼","赫连","黑齿","黑肱","侯冈","呼延","壶丘","呼衍","斛律","胡非","胡母","胡毋","皇甫","皇父","兀官","吉白","即墨","季瓜","季连","季孙","茄众","蒋丘","金齿","晋楚","京城","泾阳","九百","九方","睢鸠","沮渠","巨母","勘阻","渴侯","渴单","可汗","空桐","空相","昆吾","老阳","乐羊","荔菲","栎阳","梁丘","梁由","梁馀","梁垣","陵阳","伶舟","冷沦","令狐","刘王","柳下","龙丘","卢妃","卢蒲","鲁步","陆费","角里","闾丘","马矢","麦丘","茅夷","弥牟","密革","密茅","墨夷","墨台","万俊","昌顿","慕容","木门","木易","南宫","南郭","南门","南荣","欧侯","欧阳","逄门"};
    string randomMing[1000] = {"含蕊","亦玉","靖荷","碧萱","寒云","向南","书雁","怀薇","思菱","忆文","翠巧","怀山","若山","向秋","凡白","绮烟","从蕾","天曼","又亦","依琴","曼彤","沛槐","又槐","元绿","安珊","夏之","易槐","宛亦","白翠","丹云","问寒","易文","傲易","青旋","思真","妙之","半双","若翠","初兰","怀曼","惜萍","初之","宛丝","寄南","小萍","幻儿","千风","天蓉","雅青","寄文","代天","春海","惜珊","向薇","冬灵","惜芹","凌青","谷芹","香巧","雁桃","映雁","书兰","盼香","向山","寄风","访烟","绮晴","傲柔","寄容","以珊","紫雪","芷容","书琴","寻桃","涵阳","怀寒","易云","采蓝","代秋","惜梦","尔烟","谷槐","怀莲","涵菱","水蓝","访冬","半兰","又柔","冬卉","安双"};
    string randomedName = randomXing[rand()%200] + randomMing[rand()%80];
    CCLabelTTF* BotName = (CCLabelTTF*)matchLayer->getChildByTag(995);
    BotName->setString(randomedName.c_str());
}

void RankScene::closeMatchLayer(){
    CCActionInterval* fadeOut = CCFadeOut::create(0.2);
    CCCallFunc* remove = CCCallFuncN::create(this, callfuncN_selector(RankScene::removeThis));
    CCSequence* seq = CCSequence::create(fadeOut,remove,NULL);
    matchLayer->runAction(seq);
}

void RankScene::removeThis(CCNode* sender){     removeChild(sender,true);    }

void RankScene::getRobotInformation(){
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://115.29.168.228/robots");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, callfuncND_selector(RankScene::getRobotInformationCompleted));
    string postDataStr = "token=" + MainUser->UserTokenStr;
    request->setRequestData(postDataStr.c_str(), postDataStr.length());
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::getRobotInformationCompleted(CCNode* sender, void* data){
    string bufferStr = "";
    tmpBotName = "";
    tmpBotNickName = "";
    tmpBotID = "";
    tmpBotToken = "";
    tmpBotRankStr = "";
    tmpBotBaseRankScoreStr = "";
    tmpBotHeroIDStr = "";
    tmpBotHeroLevelStr = "";
    for (int i = 0 ; i < 50 ; i++){
        tmpBotAllHeroLevelStr[i] = "";
        tmpBotAllHeroLevel[i] = -1;
    }
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    for (unsigned int i = 0; i < buffer->size(); i++){
    
        //1.ID
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'i' && (*buffer)[i+2] == 'd' && (*buffer)[i+3] == '"') {
            for(int j = i+5 ; (*buffer)[j] != ',' ; j++){   tmpBotID += (*buffer)[j];   }
        }
        //2.name
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'n' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'm' && (*buffer)[i+4] == 'e' && (*buffer)[i+5] == '"' && (*buffer)[i+6] == ':') {
            for(int j = i+8 ; (*buffer)[j] != '"' ; j++){   tmpBotName += (*buffer)[j];   }
        }
        //3.token
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 't' && (*buffer)[i+2] == 'o' && (*buffer)[i+3] == 'k') {
            for(int j = i+9 ; (*buffer)[j] != '"' ; j++){   tmpBotToken += (*buffer)[j];   }
        }
        //4.answer_ai
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'a' && (*buffer)[i+2] == 'n' && (*buffer)[i+3] == 's') {
            tmpBotAnswerLV = atoi(&(*buffer)[i+12]);
        }
        //5.item_ai
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'i' && (*buffer)[i+2] == 't' && (*buffer)[i+3] == 'e') {
            tmpBotItemLV = atoi(&(*buffer)[i+10]);
        }
        //6.nick name
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'n' && (*buffer)[i+2] == 'i' && (*buffer)[i+3] == 'c') {
            for(int j = i+8 ; (*buffer)[j] != '"' ; j++){   tmpBotNickName += (*buffer)[j];   }
        }
        //7.rank
        if ((*buffer)[i] == 'k' && (*buffer)[i+1] == '_' && (*buffer)[i+2] == 's' && (*buffer)[i+3] == 'c') {
            for(int j = i+9 ; (*buffer)[j] != ',' ; j++){   tmpBotRankStr += (*buffer)[j];   }
        }
        //8.基础rank分
        if ((*buffer)[i] == 'e' && (*buffer)[i+1] == '_' && (*buffer)[i+2] == 's' && (*buffer)[i+3] == 'c') {
            for(int j = i+9 ; (*buffer)[j] != ',' ; j++){   tmpBotBaseRankScoreStr += (*buffer)[j];   }
        }
        //9.随机到的角色
        if ((*buffer)[i] == '_' && (*buffer)[i+1] == 'r' && (*buffer)[i+2] == 'o' && (*buffer)[i+3] == 'l') {
            for(int j = i+20 ; (*buffer)[j] != ',' ; j++){   tmpBotHeroIDStr += (*buffer)[j];   }
        }
        //10.gold
        if ((*buffer)[i] == '"' && (*buffer)[i+1] == 'g' && (*buffer)[i+2] == 'o' && (*buffer)[i+3] == 'l' && (*buffer)[i+4] == 'd' && (*buffer)[i+3] == '"') {
            for(int j = i+7 ; (*buffer)[j] != ',' ; j++){   tmpBotGoldStr += (*buffer)[j];   }
        }
        //11.第1个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '0' && (*buffer)[i+14] == ','){
            if (tmpBotAllHeroLevelStr[0] != "") {   tmpBotAllHeroLevelStr[0] = "";      }
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  tmpBotAllHeroLevelStr[0] += (*buffer)[j]; }
        }
        //12.第2个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '1' && (*buffer)[i+14] == ','){
            if (tmpBotAllHeroLevelStr[1] != "") {   tmpBotAllHeroLevelStr[1] = "";      }
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  tmpBotAllHeroLevelStr[1] += (*buffer)[j]; }
        }
        //13.第3个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '2' && (*buffer)[i+14] == ','){
            if (tmpBotAllHeroLevelStr[2] != "") {   tmpBotAllHeroLevelStr[2] = "";      }
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  tmpBotAllHeroLevelStr[2] += (*buffer)[j]; }
        }
        //14.第4个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '3' && (*buffer)[i+14] == ','){
            if (tmpBotAllHeroLevelStr[3] != "") {   tmpBotAllHeroLevelStr[3] = "";      }
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  tmpBotAllHeroLevelStr[3] += (*buffer)[j]; }
        }
        //15.第5个英雄的等级情况
        if ((*buffer)[i] == '{' && (*buffer)[i+1] == '"' && (*buffer)[i+2] == 'a' && (*buffer)[i+3] == 'v' && (*buffer)[i+4] == 'a' && (*buffer)[i+13] == '4' && (*buffer)[i+14] == ','){
            if (tmpBotAllHeroLevelStr[4] != "") {   tmpBotAllHeroLevelStr[4] = "";      }
            for (int j = i+23;(*buffer)[j] != '}' ; j++) {  tmpBotAllHeroLevelStr[4] += (*buffer)[j]; }
        }
    }
    //7.rank
    tmpBotRank = atoi(tmpBotRankStr.c_str());
    //8.基础rank分
    tmpBotBaseRankScore = atoi(tmpBotBaseRankScoreStr.c_str());
    //9.Hero ID
    tmpBotHeroID = atoi(tmpBotHeroIDStr.c_str())+1;
    //10.gold
    tmpBotGold = atoi(tmpBotGoldStr.c_str());
    //11.五个英雄的等级
    for (int i = 0 ; i < 5; i++){
        tmpBotAllHeroLevel[i] = atoi(tmpBotAllHeroLevelStr[i].c_str())+1;
        cout<<tmpBotAllHeroLevel[i]<<endl;
    }
    tmpBotHeroLevel = tmpBotAllHeroLevel[tmpBotHeroID-1];
    
    cout<<"机器人id为:"<<tmpBotID<<endl;
    cout<<"机器人用户名为"<<tmpBotName<<endl;
    cout<<"机器人的token为"<<tmpBotToken<<endl;
    cout<<"机器人的answer等级为"<<tmpBotAnswerLV<<endl;
    cout<<"机器人的道具能力等级为"<<tmpBotItemLV<<endl;
    cout<<"机器人的昵称为"<<tmpBotNickName<<endl;
    cout<<"机器人的rank分为"<<tmpBotRank<<endl;
    cout<<"机器人的rank基础分为"<<tmpBotBaseRankScore<<endl;
    cout<<"机器人使用的英雄编码为"<<tmpBotHeroID<<endl;
    cout<<"机器人使用的英雄等级为"<<tmpBotHeroLevel<<endl;
    cout<<"机器人的金币为"<<tmpBotGold<<endl;
    cout<<"五个英雄的等级为"<<tmpBotAllHeroLevel[0]<<" "<<tmpBotAllHeroLevel[1]<<" "<<tmpBotAllHeroLevel[2]<<" "<<tmpBotAllHeroLevel[3]<<" "<<tmpBotAllHeroLevel[4]<<endl;

    getRandomName();
}

void RankScene::getRandomName(){
    CCHttpRequest* request = new CCHttpRequest();
    string UrlData = "http://115.29.168.228/users/random_nick?user[gender]=male&token=" + tmpBotToken;
    request->setUrl(UrlData.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankScene::getRandomNameRequestCompleted));
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankScene::getRandomNameRequestCompleted(cocos2d::CCNode *sender, void *data){
    
    CCHttpResponse* response = (CCHttpResponse*)data;
    if (!response) {    return;     }
    int statusCode = response->getResponseCode();
    std::vector<char> *buffer = response->getResponseData();
    printJson(buffer);
    
    tmpBotNickName = "";
    for (unsigned int i = 0; i < buffer->size(); i++) {
        if ((*buffer)[i] == ':' && (*buffer)[i+1] == '"')
            for (int j = i + 2 ; (*buffer)[j] != '"' ; j++) {   tmpBotNickName += (*buffer)[j]; }
    }
    
    schedule(schedule_selector(RankScene::BotMatched),3);
}

void RankScene::BotMatched(){
        
    unschedule(schedule_selector(RankScene::setHeroCard));
    unschedule(schedule_selector(RankScene::BotMatched));
    
    string texName = "card_" + int2string(tmpBotHeroID) + ".png";
    CCSprite* BotCard = (CCSprite*)matchLayer->getChildByTag(996);
    BotCard->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(texName.c_str()));
    
    CCLabelTTF* BotName = (CCLabelTTF*)matchLayer->getChildByTag(995);
    BotName->setString(tmpBotNickName.c_str());

    CCLabelTTF* matching = (CCLabelTTF*)matchLayer->getChildByTag(994);
    matching->setString("已找到对手，即将开始游戏");
    
    CCSprite* vs = CCSprite::createWithSpriteFrameName("VS.png");
    vs->setScale(10);
    vs->setPosition(ccp(size.width/2,size.height*55/100+winDif*alpha*2));
    matchLayer->addChild(vs,103);
    
    CCActionInterval* scaleDown = CCScaleTo::create(0.3, 1);
    vs->runAction(scaleDown);
    
    MinusEnergyHTTP();
}

void RankScene::BeginOnLineGame(){
    unschedule(schedule_selector(RankScene::BeginOnLineGame));
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, GameBaseClass::scene()));
}