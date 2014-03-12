#include "ASUser.h"
#include "global.h"

int ASUser::calculateRank(int _id,int _winTimes,int _loseTimes){
    
    WinTimes[_id] = _winTimes;
    LoseTimes[_id] = _loseTimes;
    
    if(WinTimes[_id] + LoseTimes[_id] <= 10)
        return 100*(WinTimes[_id] + LoseTimes[_id]);
    else
        WinPercent[_id] = (float)WinTimes[_id]/((float)WinTimes[_id] + (float)LoseTimes[_id]);
    
    //计算rank ＝ 基础分 ＋ 胜率基本分*胜场系数*1.2 ＋ 胜场分
    
    //1.基本分
    int BaseRankScore = 1000;
    //2.胜率分 ＝ 胜率基本分*胜场系数*1.2
    int BaseWinPercentScore,WinPercentScore;
    if (WinPercent[_id]*100 >= 50)
        BaseWinPercentScore = 750 + 100* pow((double)(WinPercent[_id]*100 - 50),0.7);
    else
        BaseWinPercentScore = 750 - 100* pow((double)(50 - WinPercent[_id]*100),0.7);
    
    if (WinTimes[_id] < 20)
        //WinPercentScore = BaseWinPercentScore*1.2*(float)WinTimes[_id]/20.;
        WinPercentScore = BaseWinPercentScore*1.2*0.5;
    else if(WinTimes[_id] >= 20 && WinTimes[_id] < 50)
        WinPercentScore = BaseWinPercentScore*1*1.2;
    else if(WinTimes[_id] >= 50 && WinTimes[_id] < 100)
        WinPercentScore = BaseWinPercentScore*1.1*1.2;
    else if(WinTimes[_id] >= 100 && WinTimes[_id] < 150)
        WinPercentScore = BaseWinPercentScore*1.15*1.2;
    else if(WinTimes[_id] >= 150 && WinTimes[_id] < 300)
        WinPercentScore = BaseWinPercentScore*1.2*1.2;
    else if(WinTimes[_id] >= 300 && WinTimes[_id] < 500)
        WinPercentScore = BaseWinPercentScore*1.25*1.2;
    else if(WinTimes[_id] >= 500)
        WinPercentScore = BaseWinPercentScore*1.3*1.2;
    
    cout<<WinPercentScore<<endl;
    //3.胜场分
    int WinTimesScore = WinTimes[_id]*3 + (BaseRankScore-900)*1.5;
    if(WinTimesScore >= 3500)
        WinTimesScore = 3500;
    
    int tmp = BaseRankScore + WinPercentScore + WinTimesScore;
    if (tmp < 0)
        tmp = 0;
    return tmp;
}
