//
//  CommonRequestManager.h
//  LXNetworking
//
//  Created by liuxin on 2019/1/7.
//  Copyright © 2019年 liuxin. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LXNetworking.h"

NS_ASSUME_NONNULL_BEGIN

@interface CommonRequestManager : NSObject

+ (instancetype _Nullable )sharedInstance;

@property (nonatomic, strong) NSString * _Nullable baseURL;

/**
 清除指定本地的网络缓存数据
 
 @param urlString 网络请求的url，删除该urlString下所有的缓存（无视parameters参数）
 */
- (void)clearRequestCache:(NSString *_Nullable)urlString;

/**
 清除指定本地的网络缓存数据
 @param urlString 网络请求的url
 @param identifier 该次请求的唯一表示id，比如楼盘id、个人信息id （可以不写，则删除该urlString下所有的缓存（无视parameters参数））
 */
- (void)clearRequestCache:(NSString *_Nullable)urlString identifier:(NSString *_Nullable)identifier;

/**
 清除所有本地的网络缓存数据
 */
- (void)clearAllCache;

#pragma mark - 具体接口
- (void)getAddressWithIP:(NSString *)ip
                   cache:(LXRequestManagerCache _Nullable )cache
                 success:(LXRequestManagerSuccess _Nullable )success
                 failure:(LXRequestManagerSuccess _Nullable )failure;

-(void)getWeatherWithCityName:(NSString *)cityName
                        cache:(LXRequestManagerCache _Nullable )cache
                      success:(LXRequestManagerSuccess _Nullable )success
                      failure:(LXRequestManagerSuccess _Nullable )failure;

@end

NS_ASSUME_NONNULL_END
