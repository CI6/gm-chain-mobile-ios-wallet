//
//  AppDelegate.m
//  gmc
//
//  Created by AnonymityMaster on 2020/2/7.
//  Copyright © 2020 ci6. All rights reserved.
//

#import "AppDelegate.h"
#import "LaunchController.h"
#include "libs/secp256k1/secp256k1.h"

#include "test/TestGmcWallet.h"


@interface AppDelegate ()

@end

@implementation AppDelegate

/**
 *  获取上下文
 */
static secp256k1_context* get_static_context()
{
    static secp256k1_context* ctx_both = 0;
    if (!ctx_both){
        ctx_both  = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN);
    }
    return ctx_both;
}


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    

    NSLog(@"%@ starting ...",@"Hello GMC APP");
    
    // test secp256k1 call
    secp256k1_context* ctx_both = get_static_context();
    
    printf("secp256k1_context address -> %p\n",ctx_both);
    
    // test json serialize
    NSDictionary* dict = @{ @"a": @"123", @"b": @"你好xxxxx" };
    NSError* err = nil;
    NSData* data_dict = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONReadingAllowFragments error:&err];
    const unsigned char* data_bytes = (const unsigned char*)[data_dict bytes];
    const unsigned char* data_bytes2 = "{\"a\":\"123\",\"b\":456,\"c\":\"你好!\"}";
    
    // test
    testMain();
    
    // Launch Controller
    LaunchController* vc = [[LaunchController alloc] init];

    // 创建 window
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    [self createMainWindow:YES];
    [self.window setRootViewController:vc];
    [self.window makeKeyAndVisible];
    
    return YES;
}

-(UIWindow*)createMainWindow:(BOOL)force_reset
{
    if (self.window && !force_reset)
        return self.window;
    
    ///<    初始化窗口
    if (!self.window){
        self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    }
    
    return self.window;
}

//
//#pragma mark - UISceneSession lifecycle
//
//
//- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options {
//    // Called when a new scene session is being created.
//    // Use this method to select a configuration to create the new scene with.
//    return [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
//}
//
//
//- (void)application:(UIApplication *)application didDiscardSceneSessions:(NSSet<UISceneSession *> *)sceneSessions {
//    // Called when the user discards a scene session.
//    // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
//    // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
//}


@end
