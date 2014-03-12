#include "ASGame.h"
#include <sys/timeb.h>

int ASGame::calDamage(){
    
    struct timeb tp;
    ftime(&tp);
    srand(tp.millitm);
    
    //1.计算爆击率
    float _criticalRate = 0.05*(moveCount-moveToFight) + criticalRateAdded;
    if (_criticalRate > 1)
        _criticalRate = 1;
    
    //2.计算爆击伤害系数
    float _criticalDamageAlpha;
    if (_criticalRate == 0)
        _criticalDamageAlpha = 0;
    else{
        int moveCountAdded = criticalRateAdded/0.05;
        _criticalDamageAlpha = ((float)(moveCount+moveCountAdded)/moveToFight - (1-_criticalRate))/_criticalRate;
    }


    //剑圣的爆击系数更高
    if (rolerTag == 5)
        _criticalDamageAlpha = _criticalDamageAlpha * 1.2 + 0.05 * (Level-10);

    //3.随机浮动比例伤害
    int floatDamageRateInt = rand()%5;
    int floatDamageRateFloat = rand()%10;
    float floatRate = (float)floatDamageRateInt + (float)floatDamageRateFloat/10;
    
    //3.随机是否爆击
    int res = rand()%100 + 1;
    
    //剑圣有攻击力加成
    int tmpBasicAttack = basicAttack;
    if (rolerTag == 5 && Level >= 10)
        tmpBasicAttack = basicAttack * (1.1 + (Level-1)*0.005);
    
    //计算伤害
    if (res <= _criticalRate*100) {
        isCritical = true;
        return tmpBasicAttack * _criticalDamageAlpha * (1-floatRate/100);
    }else{
        isCritical = false;
        return tmpBasicAttack * (1-floatRate/100);
    }
}