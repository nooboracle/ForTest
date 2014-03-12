#include<iostream>
using namespace std;
class IOSBridge
{
public:
    //player
    static void PlayAudioIOS(string audioName);
    static void StopAudioIOS();

    //weixin
    static bool WXImageIOS(int index);
    
    //video
    static void PlayVideoIOS();
    static void playSectionVideoIOS();
    
    //duomeng ad
    static void chapingAD();
    static void bannerAD();
};