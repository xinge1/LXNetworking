//
//  CommonRequestManager.m
//  LXNetworking
//
//  Created by liuxin on 2019/1/7.
//  Copyright © 2019年 liuxin. All rights reserved.
//

#import "CommonRequestManager.h"
//#import "LXNetworking.h"

@interface CommonRequestManager ()

@property (nonatomic, strong) LXNetworkingManager *requestConvertManager;

@end


@implementation CommonRequestManager

#pragma mark - 初始化管理
+ (instancetype)sharedInstance {
    static id instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
    });
    return instance;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.requestConvertManager = [LXNetworkingManager sharedInstance];
        [self initialConfig];
    }
    return self;
}

- (void)initialConfig {
    
    self.requestConvertManager.configuration.baseURL = @"http://ip.taobao.com/";
//    self.requestConvertManager.configuration.resultCacheDuration = 1;
    self.requestConvertManager.configuration.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json", @"text/html", @"text/json", @"text/plain", @"text/javascript", @"text/xml", @"image/*", nil];
    [self.requestConvertManager setLoggerLevel:AFLoggerLevelInfo];
    //通过configuration来统一处理输出的数据，比如对token失效处理、对需要重新登录拦截
    self.requestConvertManager.configuration.resposeHandle = ^id (NSURLSessionTask *dataTask, id responseObject) {
        return responseObject;
    };
}

- (void)setBaseURL:(NSString *)baseURL {
    _baseURL = baseURL;
    self.requestConvertManager.configuration.baseURL = baseURL;
}


#pragma mark - 缓存管理
- (void)clearRequestCache:(NSString *_Nullable)urlString {
    [_requestConvertManager clearRequestCache:urlString];
}

- (void)clearRequestCache:(NSString *_Nullable)urlString identifier:(NSString *_Nullable)identifier {
    [_requestConvertManager clearRequestCache:urlString identifier:identifier];
}

- (void)clearAllCache {
    [_requestConvertManager clearAllCache];
}

#pragma mark - 具体接口
- (void)getAddressWithIP:(NSString *)ip
                   cache:(LXRequestManagerCache _Nullable )cache
                 success:(LXRequestManagerSuccess _Nullable )success
                 failure:(LXRequestManagerSuccess _Nullable )failure {
    

    
    NSDictionary *params = @{@"ip":ip};
    [self.requestConvertManager requestMethod:LXRequestMethodGet URLString:@"service/getIpInfo.php" parameters:params configurationHandler:^(LXNetworkingConfig * _Nullable configuration) {
        
        
        
    } cache:^(id  _Nullable responseObject) {
        
        cache(responseObject);
        
    } success:^(NSURLSessionTask * _Nullable httpbase, id  _Nullable responseObject) {
        
        success(httpbase, responseObject);
        
    } failure:^(NSURLSessionTask * _Nullable httpbase, LXError * _Nullable error) {
        
        failure(httpbase, error);
        
    }];
    
}

-(void)getWeatherWithCityName:(NSString *)cityName
                        cache:(LXRequestManagerCache _Nullable )cache
                      success:(LXRequestManagerSuccess _Nullable )success
                      failure:(LXRequestManagerSuccess _Nullable )failure{
 
    //https://www.sojson.com/open/api/weather/json.shtml?city=%E5%8C%97%E4%BA%AC
    NSDictionary *params = @{@"city":cityName};
    [self.requestConvertManager requestMethod:LXRequestMethodGet URLString:@"open/api/weather/json.shtml" parameters:params configurationHandler:^(LXNetworkingConfig * _Nullable configuration) {
        
        configuration.baseURL = @"https://www.sojson.com/";
        configuration.requestCachePolicy = LXRequestReturnCacheOrLoadToCache;
        configuration.resultCacheDuration = 10;
        
    } cache:^(id  _Nullable responseObject) {
        
        cache(responseObject);
        
    } success:^(NSURLSessionTask * _Nullable httpbase, id  _Nullable responseObject) {
        
        success(httpbase, responseObject);
        
    } failure:^(NSURLSessionTask * _Nullable httpbase, LXError * _Nullable error) {
        
        failure(httpbase, error);
        
    }];
    
}

@end
