//
//  LXNetworkCache.m
//  AFNetworking
//
//  Created by liuxin on 2019/1/9.
//

#define LXCouponKey_sign        @"sign"
#define LXCouponKey_timestamp   @"timestamp"

#import "LXNetworkCache.h"
#import "YYCache.h"

static NSString *const LXNetworkResponseCache = @"LXNetworkResponseCache";
static NSString *const LXNetworkResponseCacheTimeOut = @"LXNetworkResponseCacheTimeOut";

@implementation LXNetworkCache
static YYCache *_dataCache;

+ (void)initialize {
    _dataCache = [YYCache cacheWithName:LXNetworkResponseCache];
}


/**
 写入缓存

 */
+ (void)setHttpCache:(id)httpData
                 URL:(NSString *)URL
          parameters:(NSDictionary *)parameters{
    NSString *cacheKey = [self cacheKeyWithURL:URL parameters:[self detailNOCareParams:parameters]];
    NSLog(@"写入缓存 [url] === [%@], [cacheKey] = [%@]",URL,cacheKey);
    //异步缓存,不会阻塞主线程
    [_dataCache setObject:httpData forKey:cacheKey withBlock:nil];
    //缓存请求过期时间
    [self setCacheInvalidTimeWithCacheKey:cacheKey];
   
}


/**
 获取缓存

 */
+ (id)httpCacheForURL:(NSString *)URL
           parameters:(NSDictionary *)parameters
       cacheValidTime:(NSTimeInterval)cacheValidTime{
    NSString *cacheKey = [self cacheKeyWithURL:URL parameters:[self detailNOCareParams:parameters]];
    id cache = [_dataCache objectForKey:cacheKey];

    if (!cache) {
        return nil;
    }
    
    if ([self verifyInvalidCache:cacheKey resultCacheDuration:cacheValidTime]) {
        return cache;
    }else{
        [_dataCache removeObjectForKey:cacheKey];
        NSString *cacheDurationKey = [NSString stringWithFormat:@"%@_%@",cacheKey, LXNetworkResponseCacheTimeOut];
        [_dataCache removeObjectForKey:cacheDurationKey];
        return nil;
    }
    
}

+ (NSInteger)getAllHttpCacheSize {
    return [_dataCache.diskCache totalCost];
}

+ (void)removeAllHttpCache {
    
    [_dataCache removeAllObjectsWithBlock:^{
        
    }];
}

+(void)removeHttpCacheWithUrl:(NSString *)url
                   parameters:(NSDictionary *)parameters{
    
    NSString *cacheKey = [self cacheKeyWithURL:url parameters:[self detailNOCareParams:[self detailNOCareParams:parameters]]];
    [_dataCache removeObjectForKey:cacheKey withBlock:^(NSString * _Nonnull key) {
        NSLog(@"key = %@",key);
    }];
}


+ (NSString *)cacheKeyWithURL:(NSString *)URL parameters:(NSDictionary *)parameters {
    if(!parameters || parameters.count == 0){return URL;};
    // 将参数字典转换成字符串
    NSData *stringData = [NSJSONSerialization dataWithJSONObject:parameters options:0 error:nil];
    NSString *paraString = [[NSString alloc] initWithData:stringData encoding:NSUTF8StringEncoding];
    NSString *cacheKey = [NSString stringWithFormat:@"%@%@",URL,paraString];
    
    return [NSString stringWithFormat:@"%@",cacheKey];
}


/**
 因为淘客链接每次请求都要加时间，到时参数不一样。所以存取缓存要忽略此参数
 */
+(NSDictionary *)detailNOCareParams:(NSDictionary *)params{
    NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithDictionary:params];
    [dic removeObjectsForKeys:@[LXCouponKey_sign,LXCouponKey_timestamp]];
    return dic.copy;
}

/**
 存入缓存创建时间
 */
+ (void)setCacheInvalidTimeWithCacheKey:(NSString *)cacheKey{
    
    NSString *cacheDurationKey = [NSString stringWithFormat:@"%@_%@",cacheKey, LXNetworkResponseCacheTimeOut];
    NSTimeInterval nowTime = [[NSDate date] timeIntervalSince1970];
//    NSTimeInterval invalidTime = nowTime + resultCacheDuration;
    [_dataCache setObject:@(nowTime) forKey:cacheDurationKey withBlock:nil];
}

/**
 判断缓存是否有效，有效则返回YES
 */
+ (BOOL)verifyInvalidCache:(NSString *)cacheKey
       resultCacheDuration:(NSTimeInterval )resultCacheDuration{
    //获取该次请求失效的时间戳
    NSString *cacheDurationKey = [NSString stringWithFormat:@"%@_%@",cacheKey, LXNetworkResponseCacheTimeOut];
    id createTime = [_dataCache objectForKey:cacheDurationKey];
    NSTimeInterval createTime1 = [createTime doubleValue];
    NSTimeInterval nowTime = [[NSDate date] timeIntervalSince1970];
    if ((nowTime - createTime1) < resultCacheDuration) {
        return YES;
    }
    return NO;
}

@end
