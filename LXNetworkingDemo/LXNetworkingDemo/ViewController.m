//
//  ViewController.m
//  LXNetworkingDemo
//
//  Created by liuxin on 2019/1/8.
//  Copyright © 2019年 liuxin. All rights reserved.
//

#import "ViewController.h"
#import "CommonRequestManager.h"

@interface ViewController ()

@property (nonatomic , strong) CommonRequestManager *requestManager;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.requestManager = [CommonRequestManager sharedInstance];
}

- (IBAction)sendRequest:(id)sender {

    [self.requestManager getWeatherWithCityName:@"北京" cache:^(id  _Nullable responseObject) {
        
        NSLog(@"cache === %@",responseObject);
    } success:^(NSURLSessionTask * _Nullable httpbase, id  _Nullable responseObject) {
        
        NSLog(@"request === %@",responseObject);
    } failure:^(NSURLSessionTask * _Nullable httpbase, id  _Nullable responseObject) {
        
        NSLog(@"failure === %@",responseObject);
    }];
    
}


@end
