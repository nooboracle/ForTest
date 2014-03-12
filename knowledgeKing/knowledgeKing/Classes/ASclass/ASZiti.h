#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class AsZiti: public CCLabelBMFont{
public:
    
    static CCPoint autoCorrecttPosition(string number , CCPoint position , int scale);
    static void rollNumber(CCLabelBMFont* _label ,int _number);
    void rollNumberCounter();
    
};


static CCPoint autoCorrecttPosition(string number , CCPoint position , int scale){

    int len = number.length();
    position.x -= scale*20*len/2;
    
    return position;
}

/*
static void rollNumber(CCLabelBMFont* _label ,int _number){
    
    int number = atoi(_label->getString());
    int step = number/10;
    
    schedule(schedule_selector(ASZiti::rollNumberCounter),0.1);
    //_label->setString(number)
    
}

void ASZiti::rollNumberCounter(){
    
}
*/