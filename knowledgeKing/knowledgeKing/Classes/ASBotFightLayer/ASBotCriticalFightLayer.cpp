#include "ASBotFightLayer.h"
using namespace std;

void ASBotFightLayer::initCriticalFightLayer(int _MainIndex,int _BotIndex,bool _isCritical,int _damage,int _playerNumber,int _moveCount,int _shengming,int _botMoveCount,int _botShengMing){
    
    MainIndex = _MainIndex;
    BotIndex = _BotIndex;
    damage = _damage;
    playerNumber = _playerNumber;
    moveCount = _moveCount;
    shengming = _shengming;
    botMoveCount = _botMoveCount;
    botShengMing = _botShengMing;
    isCritical = _isCritical;
    
    showHeroInformation();
    
    if (MainIndex == 1)
        InitIM();
    else if(MainIndex == 2)
        InitMK();
    else if(MainIndex == 3)
        InitLK();
    else if(MainIndex == 4)
        InitSD();
    else if(MainIndex == 5)
        InitBM();
    else if(MainIndex == 6)
        InitLF();
    
   
}