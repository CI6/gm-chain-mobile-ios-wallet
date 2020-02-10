//
//  LaunchController.m
//  gmc
//
//  Created by AnonymityMaster on 2020/2/7.
//  Copyright © 2020 ci6. All rights reserved.
//

#import "LaunchController.h"

@implementation LaunchController


- (void)dealloc
{
}

- (id)init
{
    self = [super init];
    if (self)
    {
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // 欢迎界面
    CGRect screenRect = [[UIScreen mainScreen] bounds];

    UILabel* lb = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, screenRect.size.width, 100)];
    lb.text = @"HELLO GMC";
    lb.textColor = [UIColor blueColor];
    
    self.view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:lb];
    
    //  启动初始化
    [self startInit:YES];
}

/**
 *  启动初始化
 */
- (void)startInit:(BOOL)first_init
{

}

/**
 * 进入主界面
*/
- (void)enterMainController
{
    
}

@end
