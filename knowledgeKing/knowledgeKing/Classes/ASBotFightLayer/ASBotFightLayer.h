#include "cocos2d.h"
using namespace cocos2d;

class ASBotFightLayer:public CCLayer{
public:
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size = CCSize(640,854);
    float winDif = (((winSize.height - 854)/2)/854)*400;
    float alpha = winSize.height/854;
    
    //以下不移动
    CCSprite* fightBG; //战斗大背景:不移动
    CCSprite* halo; //光晕：不移动
    CCParticleSystemQuad* flies; //飞翔的线条
    
    CCParticleSystemQuad* skillEffect_pre; //前摇
    CCParticleSystemQuad* skillEffect_fly; //技能飞行过程中的特效
    CCSprite* skillEffectSprite_fly;
    CCParticleSystemQuad* skillEffect_hit; //技能击中特效

    
    //战斗提示
    CCSprite* MainImage;
    CCLabelTTF* MainHP;
    CCLabelTTF* MainSP;
    CCSprite* BotImage;
    CCLabelTTF* BotHP;
    CCLabelTTF* BotSP;
    CCLabelTTF* minusHP;  //减血提示
    
    //以下移动
    CCLayer* CameraLayer; //镜头层
    CCSprite* MainHero; //我方英雄
    CCSprite* BotHero;  //敌方英雄
    CCLabelTTF* Label;  //恶搞背景标语
    CCLabelTTF* activeSkillName; //主动技能名称
    CCLabelTTF* passiveSkillName; //被动技能名称
    
    CCSprite* blade;
    CCSprite* mouse[3];
    CCSprite* sword;
    CCSprite* IceCube[6];
    
    int MainIndex;
    int BotIndex;
    int damage;
    int playerNumber;
    int moveCount;
    int shengming;
    int botMoveCount;
    int botShengMing;
    bool isCritical = false;
    
    static ASBotFightLayer* createAsFightLayer(
                                            int _MainIndex,int _BotIndex,bool _isCritical,int _damage,int _playerNumber,
                                            int _moveCount,int _shengming,int _botMoveCount,int _botShengMing
                                            );
    
    void initBasicFightLayer(int _MainIndex,int _BotIndex,bool _isCritical,int _damage,int _playerNumber,
                             int _moveCount,int _shengming,int _botMoveCount,int _botShengMing
                             );
    void initCriticalFightLayer(int _MainIndex,int _BotIndex,bool _isCritical,int _damage,int _playerNumber,
                                int _moveCount,int _shengming,int _botMoveCount,int _botShengMing
                                );
    void activeSkillNameAnimation();
    void passiveSkillNameAnimation();
    void backToMainGame();
    void showHeroInformation();
    void changeMainHeroInformation(int _damage, int _sp);
    void changeBotHeroInformation(int _damage, int _sp);
    
    //1.Blade master
    //1.1 普通
    void BladeMasterPreAttack();
    void BladeMasterInAttack();
    void BladeMasterHitEnemy();
    //1.2 超级
    int BMCounter = 0;
    void InitBM();
    void BladeMasterPreAttackCritical();
    void BladeMasterInAttackCritical();
    void BladeMasterHitEnemyCritical();
    void shadowSlash();
    void shadowSlashHitEnemy();
    void endShadowSlash();
    void phoenixAttack();
    void phoenixMinusHP();
    void dragonAttack();
    void dragonMinusHP();
    
    //2.Lich King
    void LichKingPreAttack();
    void LichKingInAttack();
    void LichKingHitEnemy();
    //2.2 超级
    void InitLK();
    void LichKingPreAttackCritical();
    void LichKingInAttackCritical();
    void LichKingHitEnemyCritical();
    void IceCubeFall();
    void IceCubeHit();
    void SwordFall();
    void LichKingBigAttack();
    void LichKingMinusHP();
    void removeSwordAndCube();
    
    //3.Iron Man
    //3.1 普通
    void IronManPreAttack();
    void IronManInAttack();
    void IronManHitEnemy();
    //3.2 超级
    void InitIM();
    void IronManPreAttackCritical();
    void IronManInAttackCritical();
    void EmitLight(CCNode* sender);
    void EmitSecondLight(CCNode* sender);
    void reachFirstSatelite();
    void reachSecondSatelite();
    void IronManBigAttack();
    void IronManHitEnemyCritical();
    void MinusHP();
    
    //4.Luffy
    void LuffyPreAttack();
    void LuffyInAttack();
    void LuffyAttackAnimation();
    void LuffyHitEnemy();
    //4.2 超级
    void InitLF();
    
    //5.Miku
    void MikuPreAttack();
    void MikuInAttack();
    void MikuHitEnemy(CCNode* sender);
    //5.2 超级
    int mikuCriticalCounter = 0;
    void InitMK();
    void MikuPreAttackCritical();
    void MikuInAttackCritical();
    void MikuBigAttack();
    void MikuHitEnemyCritical();

    //6.sheldon
    //6.1 普通
    void SheldonPreAttack();
    void SheldonInAttack();
    void MouseStopMoving(CCNode* sender);
    void Bazinga();
    void SheldonHitEnemy();

    //6.2 超级
    void InitSD();
    void SheldonPreAttackCritical();
    void SheldonInAttackCritical();
    void SheldonHitEnemyCritical();
    void SheldonSecondPeriodCritical();
    void SheldonSecondHitEnemy();
    void SilentAnimation();

    void removeThis(CCNode* sender);
    
};