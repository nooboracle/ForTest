#include "cocos2d.h"
#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>
#include "AsMessageBox.h"

enum{
    TianTianDaRen_Gold_1,
    TianTianDaRen_Gold_2,
    TianTianDaRen_Gold_3,
    TianTianDaRen_Gold_4,
    TianTianDaRen_Gold_5,
    TianTianDaRen_Gold_6_1
}buyCoinsTag;

@interface iapGeneral : NSObject<SKProductsRequestDelegate,SKPaymentTransactionObserver>{
    int buyType;
    AsMessageBox* box;
}

-(iapGeneral*)init;
-(void)requestProUpgradeProductData;
-(void)RequestProductData;
-(bool)CanMakePay;
-(void)buy:(int)type;
-(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction;
-(void) completeTransaction: (SKPaymentTransaction *)transaction;
-(void) failedTransaction: (SKPaymentTransaction *)transaction;
-(void) paymentQueueRestoreCompletedTransactionsFinished: (SKPaymentTransaction *)transaction;
-(void) paymentQueue:(SKPaymentQueue *) paymentQueue restoreCompletedTransactionsFailedWithError:(NSError *)error;
-(void) restoreTransaction: (SKPaymentTransaction *)transaction;
-(void)provideContent:(NSString *)product;
-(void)recordTransaction:(NSString *)product;
-(BOOL)isNetworkOK;

@end
