#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "CCVideoPlayeriOS/CCVideoPlayer.h"
#import "WXApi.h"
#include <iostream>
using namespace std;

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate,AVAudioPlayerDelegate,WXApiDelegate,CCVideoPlayerDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
    AVAudioPlayer* audioPlayer;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;
@property (strong, nonatomic) AVAudioPlayer *audioPlayer;

//audio
- (void)playAudio:(string)audioName;
- (void)stopAudio;

//weixin
- (BOOL) isWXAppInstalledBridge;
- (void) sendImageContent:(int)index;
- (void) sendMusicContent;
- (UIImage *)scaleImage:(UIImage *)image toScale:(float)scaleSize;

//video
-(void)playVideo;

@end

