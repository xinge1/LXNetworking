//
//  LXNetworkingManager.h
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

@class LXNetworkingConfig;
@class LXError;
#import "AFNetworkActivityLogger.h"

NS_ASSUME_NONNULL_BEGIN

/**
 网络请求方法
 
 - LXRequestMethodGet: GET方法，通常用于请求服务器发送某个资源，服务器返回的数据可能是缓存数据
 - LXRequestMethodHead: HEAD方法与GET方法的行为很类似，但服务器在响应中只返回首部。不会反回实体的主体部分
 - LXRequestMethodPost: POST方法起初是用来向服务器写入数据的，也可以提交表单，也可以请求获取某个资源数据
 - LXRequestMethodPut: 与GET方法从服务器读取文档相反，PUT方法会向服务器写入文档
 - LXRequestMethodPatch: PATCH 用于资源的部分内容的更新
 - LXRequestMethodDelete: DELETE方法所做的事情就是请服务器删除请求URL所指定的资源。
 */
typedef NS_ENUM(NSInteger, LXRequestMethod) {
    LXRequestMethodGet = 0,
    LXRequestMethodHead,
    LXRequestMethodPost,
    LXRequestMethodPut,
    LXRequestMethodPatch,
    LXRequestMethodDelete,
};

/**
网络状态
*/
typedef NS_ENUM(NSInteger, LXNetworkStatus) {
    /*! 未知网络 */
    LXNetworkStatusUnknown           = 0,
    /*! 没有网络 */
    LXNetworkStatusNotReachable,
    /*! 手机 3G/4G 网络 */
    LXNetworkStatusReachableViaWWAN,
    /*! wifi 网络 */
    LXNetworkStatusReachableViaWiFi
};

typedef void(^LXRequestManagerCompletion)(NSURLSessionTask * _Nullable httpbase, id _Nullable cacheResponse, id _Nullable response, LXError * _Nullable error);
typedef void(^LXRequestManagerCache)(id _Nullable responseObject);
typedef void(^LXRequestManagerSuccess)(NSURLSessionTask * _Nullable httpbase, id _Nullable responseObject);
typedef void(^LXRequestManagerFailure)(NSURLSessionTask * _Nullable httpbase, LXError * _Nullable error);
typedef void(^LXRequestManagerProgress)(NSProgress * _Nullable progress);
/*! 实时监测网络状态的 block */
typedef void(^LXNetworkStatusBlock)(LXNetworkStatus status);

/**
 网络请求中间转换类，不关心业务，将上层的请求通过该类转发给AFN、ASI等网络库，请求中的一些配置通过configuration来处理
 */
@interface LXNetworkingManager : NSObject

/**
单例初始化网络请求，该类作为一个中间转换类来处理网络请求，内部使用AFN处理，当然可以方便切换成其他网络库，
比如ASI、系统的NSURLSession，而不影响上层的业务，该类也不会加入上层的一些控制代码，
一些配置通过configuration传入
@return 网络请求管理类
*/
+ (instancetype _Nullable )sharedInstance;

/**
 *  当前的网络状态
 */
@property (nonatomic, assign) AFNetworkReachabilityStatus networkStatus;

/// 实时网络状态监测，通过Block回调实时获取
@property (nonatomic, copy) LXNetworkStatusBlock lxNetworkStatusBlock;


/**
 上层的请求配置，通过该属性传递，保证该类内部不处理上层的逻辑
 */
@property(nonatomic, strong) LXNetworkingConfig * _Nullable configuration;


/**
 设置网络请求的log等级
 
 @param loggerLevel log等级，当网络请求失败时，无论是哪种等级都会打印error信息，当网络成功时，
 AFLoggerLevelInfo：打印请求的code码、请求的url和本次请求耗时；
 AFLoggerLevelDebug/AFLoggerLevelWarn/AFLoggerLevelError：打印请求的code码、请求的url、本次请求耗时、header信息及返回数据；
 */
- (void)setLoggerLevel:(AFHTTPRequestLoggerLevel)loggerLevel;


/**
 提供给上层请求
 
 @param method 请求的方法，可以在configuration选择是否缓存数据
 @param URLString 请求的URL地址，不包含baseUrl
 @param parameters 请求的参数
 @param configurationHandler 将默认的配置给到外面，外面可能需要特殊处理，可以修改baseUrl等信息
 @param cache 如果有的话返回缓存数据（⚠️⚠️缓存的数据是服务器返回的数据，而不是经过configuration处理后的数据，但是返回给上层数据事处理后的数据）
 @param success 请求成功
 @param failure 请求失败
 @return 返回该请求的任务管理者，用于取消该次请求(⚠️⚠️，当返回值为nil时，表明并没有进行网络请求，那就是取缓存数据)
 */
- (NSURLSessionDataTask *_Nullable)requestMethod:(LXRequestMethod)method
                                       URLString:(NSString *_Nullable)URLString
                                      parameters:(NSDictionary *_Nullable)parameters
                            configurationHandler:(void (^_Nullable)(LXNetworkingConfig * _Nullable configuration))configurationHandler
                                           cache:(LXRequestManagerCache _Nullable )cache
                                         success:(LXRequestManagerSuccess _Nullable )success
                                         failure:(LXRequestManagerFailure _Nullable )failure;

/**
 上传资源方法
 
 @param URLString URLString 请求的URL地址，不包含baseUrl
 @param parameters 请求参数
 @param block 将要上传的资源回调
 @param configurationHandler 将默认的配置给到外面，外面可能需要特殊处理，可以修改baseUrl等信息
 @param progress 上传资源进度
 @param success 请求成功
 @param failure 请求失败
 @return 返回该请求的任务管理者，用于取消该次请求
 */
- (NSURLSessionTask *_Nullable)uploadWithURLString:(NSString *_Nullable)URLString
                                        parameters:(NSDictionary *_Nullable)parameters
                         constructingBodyWithBlock:(void (^_Nullable)(id <AFMultipartFormData> _Nullable formData))block
                              configurationHandler:(void (^_Nullable)(LXNetworkingConfig * _Nullable configuration))configurationHandler
                                          progress:(LXRequestManagerProgress _Nullable)progress
                                           success:(LXRequestManagerSuccess _Nullable )success
                                           failure:(LXRequestManagerFailure _Nullable )failure;



/**
 下载资源方法
 
 @param URLString URLString 请求的URL地址，不包含baseUrl
 @param configurationHandler 将默认的配置给到外面，外面可能需要特殊处理，可以修改baseUrl等信息
 @param progress 上传资源进度
 @param success 请求成功
 @param failure 请求失败
 @return 返回该请求的任务管理者，用于取消该次请求
 */
- (NSURLSessionTask *_Nullable)downloadWithURLString:(NSString *_Nullable)URLString
                                configurationHandler:(void (^_Nullable)(LXNetworkingConfig * _Nullable configuration))configurationHandler
                                            progress:(LXRequestManagerProgress _Nullable)progress
                                             success:(LXRequestManagerSuccess _Nullable )success
                                             failure:(LXRequestManagerFailure _Nullable )failure;



/**
 取消所有请求
 */
- (void)cancelAllRequest;


@end

NS_ASSUME_NONNULL_END
