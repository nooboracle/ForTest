#import "IapGeneral.h"
#import <SystemConfiguration/SCNetworkReachability.h>
#import <netinet/in.h>
#include "ASUser.h"
#include "RankScene.h"
#include "AsShopLayer.h"
using namespace std;
#define ProductID_iap6 @"TianTianDaRen_Gold_1"
#define ProductID_iap18 @"TianTianDaRen_Gold_2"
#define ProductID_iap30 @"TianTianDaRen_Gold_3"
#define ProductID_iap68 @"TianTianDaRen_Gold_4"
#define ProductID_iap168 @"TianTianDaRen_Gold_5"
#define ProductID_iap328 @"TianTianDaRen_Gold_6_1"

extern int buyIndex;
extern bool bought;
extern ASUser* MainUser;
extern RankScene* RankSceneLayer;
extern AsShopLayer* shopLayer;

@interface iapGeneral ()
@end

@implementation iapGeneral

-(BOOL)isNetworkOK{
    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityFlags flags;
    
    BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
    CFRelease(defaultRouteReachability);
    
    if (!didRetrieveFlags){
        NSLog(@"Error. Could not recover network reachability flags");
        return NO;
    }
    
    BOOL isReachable = flags & kSCNetworkFlagsReachable;
    BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
    return (isReachable && !needsConnection) ? YES : NO;
}

-(void)buy:(int)type{
    if ([self isNetworkOK]) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        buyType = type;
        if ([SKPaymentQueue canMakePayments])
            [self RequestProductData];
        else{
            UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                                message:@"您不能购买这项内购产品"
                                                               delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
            [alerView show];
            [alerView release];
        }
    }
    else{
        UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                            message:@"通信超时，请检查您的网络后重试"
                                                           delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
        [alerView show];
        [alerView release];
    }
}

-(void)RequestProductData{
    NSLog(@"---------请求对应的产品信息------------");
    NSArray *product = nil;
    switch (buyType) {
        case TianTianDaRen_Gold_1:
            product=[[NSArray alloc] initWithObjects:ProductID_iap6,nil];
            break;
        case TianTianDaRen_Gold_2:
            product=[[NSArray alloc] initWithObjects:ProductID_iap18,nil];
            break;
        case TianTianDaRen_Gold_3:
            product=[[NSArray alloc] initWithObjects:ProductID_iap30,nil];
            break;
        case TianTianDaRen_Gold_4:
            product=[[NSArray alloc] initWithObjects:ProductID_iap68,nil];
            break;
        case TianTianDaRen_Gold_5:
            product=[[NSArray alloc] initWithObjects:ProductID_iap168,nil];
            break;
        case TianTianDaRen_Gold_6_1:
            product=[[NSArray alloc] initWithObjects:ProductID_iap328,nil];
        default:
            break;
    }
    NSSet *nsset = [NSSet setWithArray:product];
    SKProductsRequest *request=[[SKProductsRequest alloc] initWithProductIdentifiers: nsset];
    request.delegate=self;
    [request start];
    [product release];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    NSLog(@"-----------收到产品反馈信息--------------");
    NSArray *myProduct = response.products;
    NSLog(@"产品Product ID:%@",response.invalidProductIdentifiers);
    NSLog(@"产品付费数量: %d", [myProduct count]);
    // populate UI
    for(SKProduct *product in myProduct){
        NSLog(@"product info");
        NSLog(@"SKProduct 描述信息%@", [product description]);
        NSLog(@"产品标题 %@" , product.localizedTitle);
        NSLog(@"产品描述信息: %@" , product.localizedDescription);
        NSLog(@"价格: %@" , product.price);
        NSLog(@"Product id: %@" , product.productIdentifier);
    }
    SKPayment *payment = nil;
    switch (buyType) {
        case TianTianDaRen_Gold_1:
            payment  = [SKPayment paymentWithProductIdentifier:ProductID_iap6];
            break;
        case TianTianDaRen_Gold_2:
            payment  = [SKPayment paymentWithProductIdentifier:ProductID_iap18];
            break;
        case TianTianDaRen_Gold_3:
            payment  = [SKPayment paymentWithProductIdentifier:ProductID_iap30];
            break;
        case TianTianDaRen_Gold_4:
            payment  = [SKPayment paymentWithProductIdentifier:ProductID_iap68];
            break;
        case TianTianDaRen_Gold_5:
            payment  = [SKPayment paymentWithProductIdentifier:ProductID_iap168];
            break;
        case TianTianDaRen_Gold_6_1:
            payment  = [SKPayment paymentWithProductIdentifier:ProductID_iap328];
            break;
        default:
            break;
    }
    NSLog(@"---------发送购买请求------------");
    [[SKPaymentQueue defaultQueue] addPayment:payment];
    [request autorelease];
    
}
- (void)requestProUpgradeProductData{
    NSLog(@"------请求升级数据---------");
    NSSet *productIdentifiers = [NSSet setWithObject:@"com.productid"];
    SKProductsRequest* productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productsRequest.delegate = self;
    [productsRequest start];
}

//弹出错误信息
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error{
    NSLog(@"-------弹出错误信息----------");
    UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Alert",NULL) message:[error localizedDescription]
                                                       delegate:nil cancelButtonTitle:NSLocalizedString(@"Close",nil) otherButtonTitles:nil];
    [alerView show];
    [alerView release];
}

-(void) requestDidFinish:(SKRequest *)request{
    NSLog(@"----------反馈信息结束--------------");
}

-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction{
    NSLog(@"-----PurchasedTransaction----");
    NSArray *transactions =[[NSArray alloc] initWithObjects:transaction, nil];
    [self paymentQueue:[SKPaymentQueue defaultQueue] updatedTransactions:transactions];
    [transactions release];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
    NSLog(@"-----paymentQueue--------");
    for (SKPaymentTransaction *transaction in transactions){
        switch (transaction.transactionState){
            case SKPaymentTransactionStatePurchased:{
                [self completeTransaction:transaction];
                NSLog(@"-----交易完成 --------");
                
                if (!bought) {
                    bought = true;
                    int goldPlus[6] = {3000,9500,16500,38000,98000,198000};
                    RankSceneLayer->changeGold(goldPlus[buyIndex]);
                    RankSceneLayer->goldProcessingIndex = 3;
                    RankSceneLayer->updateUserGold();
                    shopLayer->box->closeMessageBox();
                    cout<<"现在金币有"<<MainUser->gold<<endl;

                    UIAlertView *alerView =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                                        message:@"恭喜您购买成功，请查看您的金币"
                                                                       delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
                    
                    [alerView show];
                    [alerView release];
                }
            }
                break;
            case SKPaymentTransactionStateFailed:{
                [self failedTransaction:transaction];
                NSLog(@"-----交易失败 --------");
                
                if (!bought) {
                    bought = true;
                    shopLayer->box->closeMessageBox();
                    UIAlertView *alerView2 =  [[UIAlertView alloc] initWithTitle:@"Alert"
                                                                         message:@"购买失败，请稍候重试"
                                                                        delegate:nil cancelButtonTitle:NSLocalizedString(@"Close（关闭）",nil) otherButtonTitles:nil];
                    
                    [alerView2 show];
                    [alerView2 release];
                }
            }
                break;
            case SKPaymentTransactionStateRestored:{
                [self restoreTransaction:transaction];
                NSLog(@"-----已经购买过该商品 --------");
            }
                break;
            case SKPaymentTransactionStatePurchasing:{
                NSLog(@"-----商品添加进列表 --------");
            }
                break;
            default:
                break;
        }
    }
}
- (void) completeTransaction: (SKPaymentTransaction *)transaction{
    NSLog(@"-----completeTransaction--------");
    // Your application should implement these two methods.
    NSString *product = transaction.payment.productIdentifier;
    if ([product length] > 0) {
        
        NSArray *tt = [product componentsSeparatedByString:@"."];
        NSString *bookid = [tt lastObject];
        if ([bookid length] > 0) {
            [self recordTransaction:bookid];
            [self provideContent:bookid];
        }
    }
    
    // Remove the transaction from the payment queue.
    
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
}

//记录交易
-(void)recordTransaction:(NSString *)product{
    NSLog(@"-----记录交易--------");
}

//处理下载内容
-(void)provideContent:(NSString *)product{
    NSLog(@"-----下载--------");
}

- (void) failedTransaction: (SKPaymentTransaction *)transaction{
    NSLog(@"失败");
    if (transaction.error.code != SKErrorPaymentCancelled){}
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}
- (void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction{}
- (void) restoreTransaction: (SKPaymentTransaction *)transaction{
    NSLog(@" 交易恢复处理");
}

-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error{
    NSLog(@"-------paymentQueue----");
}

#pragma mark connection delegate
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data{
    NSLog(@"%@",  [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease]);
}
- (void)connectionDidFinishLoading:(NSURLConnection *)connection{}
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response{
    switch([(NSHTTPURLResponse *)response statusCode]) {
        case 200:
        case 206:
            break;
        case 304:
            break;
        case 400:
            break;
        case 404:
            break;
        case 416:
            break;
        case 403:
            break;
        case 401:
        case 500:
            break;
        default:
            break;
    }
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    NSLog(@"test");
}

-(void)dealloc{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];//解除监听
    [super dealloc];
}
@end

