#include "cocos2d.h"
#include "IOSBridge.h"
#include "AppController.h"
#import "DMInterstitialSampleViewController.h"
#import "DMInlineSampleViewController.h"
#include<iostream>
#import <EAGLView.h>
#import "IosBridge.h"
using namespace cocos2d;
using namespace std;

void IOSBridge::PlayAudioIOS(string audioName){
    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
    [app playAudio:audioName];
}

void IOSBridge::StopAudioIOS(){
    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
    [app stopAudio];
}

bool IOSBridge::WXImageIOS(int index){
    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
    if ([app isWXAppInstalledBridge]){
        [app sendImageContent:index];
        return true;
    }
    return false;
}

void IOSBridge::PlayVideoIOS(){
    AppController *app = (AppController*) [[UIApplication sharedApplication] delegate];
    [app playVideo];
}

//ads
void IOSBridge::chapingAD(){
    UIViewController * ad = [[DMInterstitialSampleViewController alloc] init];
    [[EAGLView sharedEGLView] addSubview:ad.view];
}

void IOSBridge::bannerAD(){
    UIViewController * ad = [[DMInlineSampleViewController alloc] init];
    [[EAGLView sharedEGLView] addSubview:ad.view];
}


