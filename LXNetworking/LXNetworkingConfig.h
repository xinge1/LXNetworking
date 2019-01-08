//
//  LXNetworkingConfig.h
//  LXNetworking
//
//  Created by liuxin on 2019/1/7.
//  Copyright © 2019年 liuxin. All rights reserved.
//

#import <Foundation/Foundation.h>

#if __has_include(<AFNetworking/AFNetworking.h>)
#import <AFNetworking/AFNetworking.h>
#else
#import "AFNetworking.h"
#endif

NS_ASSUME_NONNULL_BEGIN

extern const CGFloat LXRequestTimeoutInterval;

@class HDError;


/**
 缓存策略机制
 
 - LXRequestReturnCacheDontLoad: 如果缓存有效则直接返回缓存，缓存失效则返回nil，不再load。（场景：几乎没有任何变化的接口，实效性低）
 - LXRequestReturnCacheAndLoadToCache: 如果缓存有效则直接返回缓存，并且load且缓存数据。缓存失效则load返回，且缓存数据。（场景：接口实效性不高，但需要有一定实效性，比如商品详情接口）
 - LXRequestReturnCacheOrLoadToCache: 如果缓存有效则直接返回缓存。缓存失效则load返回，且缓存数据。（场景：接口实效性不高，但需要有一定实效性，比如商品详情接口）
 - LXRequestReturnLoadToCache: 直接load并返回数据，且缓存数据，如果load失败则读取缓存数据。（场景：接口需要一定的实效性，但同时要有数据支持，比如项目的首页接口）
 - LXRequestReturnLoadDontCache: 直接load并返回数据，不缓存数据，如果load失败则直接抛出Error。（场景：接口一定是实时的，并且保证返回的数据真实、可靠、安全，而非本地缓存数据，比如支付接口）
 */
typedef NS_ENUM(NSUInteger, HDRequestCachePolicy) {
    LXRequestReturnCacheDontLoad = 0,
    LXRequestReturnCacheAndLoadToCache,
    LXRequestReturnCacheOrLoadToCache,
    LXRequestReturnLoadToCache,
    LXRequestReturnLoadDontCache,
};



@class LXError;
/**
 请求配置类，比如设置baseURL，request、response、timeout、请求的头部通用配置、缓存配置、数据做统一的处理
 */
@interface LXNetworkingConfig : NSObject

@property (nonatomic, copy) NSString *baseURL;

/**
 *  请求的一些配置（默认不变的信息），比如：缓存机制、请求超时、请求头信息等配置
 */
@property (nonatomic, strong) AFHTTPRequestSerializer *requestSerializer;

/**
 *  对返回的数据进行序列化，默认使用 AFJSONResponseSerializer，支持AFJSONResponseSerializer、AFXMLParserResponseSerializer、AFXMLDocumentResponseSerializer等
 */
@property (nonatomic, strong) AFHTTPResponseSerializer *responseSerializer;

/**
 请求的头部通用配置
 */
@property (nonatomic, strong) NSMutableDictionary *builtinHeaders;

/**
 请求体通用配置
 */
@property (nonatomic, strong) NSMutableDictionary *builtinBodys;

/**
 请求超时时间设置，默认15秒
 */
@property (nonatomic, assign) NSTimeInterval timeoutInterval;

/**
 设置缓存时间，默认86400秒（24小时）。如果 <= 0，表示不启用缓存。单位为秒，表示对于一个请求的结果缓存多长时间
 */
@property (nonatomic, assign) NSInteger resultCacheDuration;

/**
 缓存策略, 默认是HDRequestReturnLoadToCache
 */
@property (nonatomic, assign) HDRequestCachePolicy requestCachePolicy;


/**
 对请求返回的数据做统一的处理，比如token失效、重新登录等等操作。
 */
@property (nonatomic, copy) id (^ resposeHandle)(NSURLSessionTask *dataTask, id responseObject);


/**
 业务层自定义的一些error信息，方便统一管理
 */
@property (nonatomic, strong) Class LXError;

@end

NS_ASSUME_NONNULL_END
