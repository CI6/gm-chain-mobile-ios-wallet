//
//  AppDelegate.m
//  gmc
//
//  Created by kek on 2020/2/7.
//  Copyright © 2020 ci6. All rights reserved.
//

#import "AppDelegate.h"
#import "LaunchController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    NSLog(@"%@ starting ...",@"Hello GMC APP");
    
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
