#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#include "global.h"
using namespace std;
using namespace cocos2d;

bool videoFinished;
int shareImageIndex;

@implementation AppController

@synthesize window;
@synthesize viewController;
@synthesize audioPlayer;

#pragma mark -
#pragma mark Application lifecycle
static AppDelegate s_sharedApplication;
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH_COMPONENT16
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples:0 ];
    
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
    
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0){    [window addSubview: viewController.view];   }
    else    {   [window setRootViewController:viewController];  }
    [window makeKeyAndVisible];
    [[UIApplication sharedApplication] setStatusBarHidden: YES];
    cocos2d::CCApplication::sharedApplication()->run();
    [WXApi registerApp:@"wx6d1949ce6ae6e6f8"];
    [CCVideoPlayer setDelegate:self];
    return YES;
}


/* weixin */
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {   return [WXApi handleOpenURL:url delegate:self];     }
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {   return [WXApi handleOpenURL:url delegate:self];     }
-(void) onReq:(BaseReq*)req{
    if([req isKindOfClass:[GetMessageFromWXReq class]])
    {
        // 微信请求App提供内容， 需要app提供内容后使用sendRsp返回
        NSString *strTitle = [NSString stringWithFormat:@"微信请求App提供内容"];
        NSString *strMsg = @"微信请求App提供内容，App要调用sendResp:GetMessageFromWXResp返回给微信";
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle message:strMsg delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        alert.tag = 1000;
        [alert show];
        [alert release];
    }
    else if([req isKindOfClass:[ShowMessageFromWXReq class]])
    {
        ShowMessageFromWXReq* temp = (ShowMessageFromWXReq*)req;
        WXMediaMessage *msg = temp.message;
        
        //显示微信传过来的内容
        WXAppExtendObject *obj = msg.mediaObject;
        
        NSString *strTitle = [NSString stringWithFormat:@"微信请求App显示内容"];
        NSString *strMsg = [NSString stringWithFormat:@"标题：%@ \n内容：%@ \n附带信息：%@ \n缩略图:%u bytes\n\n", msg.title, msg.description, obj.extInfo, msg.thumbData.length];
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle message:strMsg delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
        [alert release];
    }
    else if([req isKindOfClass:[LaunchFromWXReq class]])
    {
        //从微信启动App
        NSString *strTitle = [NSString stringWithFormat:@"从微信启动"];
        NSString *strMsg = @"这是从微信启动的消息";
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle message:strMsg delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
        [alert release];
    }
}

- (BOOL) isWXAppInstalledBridge{return [WXApi isWXAppInstalled];}

-(void) onResp:(BaseResp*)resp{
    if([resp isKindOfClass:[SendMessageToWXResp class]])
    {
        NSString *strTitle = [NSString stringWithFormat:@"分享成功"];
        //NSString *strMsg = [NSString stringWithFormat:@"恭喜,您已获得%d金币", resp.errCode];
        NSString *strMsg = [NSString stringWithFormat:@"感谢您的分享"];
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:strTitle message:strMsg delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [alert show];
        [alert release];
    }
}

- (void) sendImageContent:(int)index{

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    //发送内容给微信
    WXMediaMessage *message = [WXMediaMessage message];

    //获得原始图片的地址
    NSString* filePath;
    string str;
    if (shareImageIndex == 0) {
        if (size.height == 854)
            str =  "ADImage_960";
        else
            str = "ADImage_" + int2string(size.height);
        NSString* imageName =  [NSString stringWithFormat:@"%s", str.c_str()];
        filePath =  [[NSBundle mainBundle]pathForResource:imageName ofType:@"png"];
    }else if(shareImageIndex == 1){
        NSArray *searchpaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsPath = [searchpaths objectAtIndex:0];
        filePath = [[NSString alloc] initWithFormat: @"%@/shared.png",documentsPath];
    }

    //压缩图片作为略缩图
    UIImage* originImage = [UIImage imageWithContentsOfFile:filePath];
    UIGraphicsBeginImageContext(CGSizeMake(originImage.size.width * 0.1, originImage.size.height * 0.1));
    [originImage drawInRect:CGRectMake(0, 0, originImage.size.width * 0.1, originImage.size.height * 0.1)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    //存储压缩后的图片
    NSString *path = [[NSHomeDirectory()stringByAppendingPathComponent:@"Documents"]stringByAppendingPathComponent:@"shared-s.png"];
    [UIImagePNGRepresentation(scaledImage) writeToFile:path atomically:YES];
    
    //设置略所图
    [message setThumbImage:[UIImage imageWithContentsOfFile:path]];
    
    
    WXImageObject *ext = [WXImageObject object];
    ext.imageData = [NSData dataWithContentsOfFile:filePath] ;
    
    message.mediaObject = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message;
    if (index == 0) {   req.scene = WXSceneSession; }
    else if (index == 1) {    req.scene = WXSceneTimeline;    }//选择发送到朋友圈，默认值为WXSceneSession，发送到会话
    
    [WXApi sendReq:req];
}

- (UIImage *)scaleImage:(UIImage *)image toScale:(float)scaleSize{
    UIGraphicsBeginImageContext(CGSizeMake(image.size.width * scaleSize, image.size.height * scaleSize));
    [image drawInRect:CGRectMake(0, 0, image.size.width * scaleSize, image.size.height * scaleSize)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return scaledImage;
}


//音频
- (void)playAudio : (string)audioName{
    NSString* AudioPlayedNameNS =  [NSString stringWithFormat:@"%s", audioName.c_str()];
    NSURL* AudioFileURL = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:AudioPlayedNameNS ofType:@"mp3"]];
    NSLog(@"IOSPlay:%@",AudioFileURL);
    audioPlayer= [[AVAudioPlayer alloc] initWithContentsOfURL:AudioFileURL error:nil];
    audioPlayer.delegate = self;
    NSLog(@"%f",audioPlayer.duration);
    if (!audioPlayer.playing) {     [audioPlayer play];     }
}
-(void)stopAudio{   if(audioPlayer.playing){    [audioPlayer stop];     }   }
-(void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag{}

//视频
-(void)playVideo{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    [CCVideoPlayer setScrrenSize:CGSizeMake(size.width/2, size.height/2)];
    [CCVideoPlayer setNoSkip:false];
    [CCVideoPlayer playMovieWithFile:@"logo.mp4"];
}
- (void) movieStartsPlaying    {
    videoFinished = false;
}
- (void) moviePlaybackFinished {
    videoFinished = true;
}


- (void)applicationWillResignActive:(UIApplication *)application {    cocos2d::CCDirector::sharedDirector()->pause();   }
- (void)applicationDidBecomeActive:(UIApplication *)application {      cocos2d::CCDirector::sharedDirector()->resume(); }
- (void)applicationDidEnterBackground:(UIApplication *)application {    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();   }
- (void)applicationWillEnterForeground:(UIApplication *)application {   cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();  }
- (void)applicationWillTerminate:(UIApplication *)application { }
#pragma mark -
#pragma mark Memory management
- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {      cocos2d::CCDirector::sharedDirector()->purgeCachedData();    }
- (void)dealloc {       [super dealloc];        }
@end

