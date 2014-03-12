#include "cocos2d.h"
#include<sstream>
using namespace std;

#define Easy_Time 20
#define tibaoAvilable 15
#define Max_sm_Easy 5
#define Combo_Time_Limit 4
#define Max_Game_Round 20

#define price_dj1 100
#define price_dj2 100
#define price_dj3 100
#define price_dj4 100
#define price_dj5 100
#define price_dj6 100
#define price_dj7 100
#define price_dj8 100
#define price_dj9 100
#define price_dj10 100

////////////////////////////////////////
static string int2string(int _number){
    std::stringstream Stream;
    std::string Str;
    Stream<<_number;
    Stream>>Str;
    return Str;
}


static string float2string(float _number){
    
    string str;
    char tmp[50];
    sprintf(tmp,"%.2f",_number);
    str.assign(tmp);
    
	return str;
}


