//
//  LXNetworkingConfig.m
//  LXNetworking
//
//  Created by liuxin on 2019/1/7.
//  Copyright © 2019年 liuxin. All rights reserved.
//

#import "LXNetworkingConfig.h"
#import "LXError.h"

const CGFloat LXRequestTimeoutInterval = 10.0f;

@implementation LXNetworkingConfig

- (instancetype)init {
    self = [super init];
    if (self) {
        _timeoutInterval = LXRequestTimeoutInterval;
        _requestCachePolicy = LXRequestReturnCacheOrLoadToCache;
        _LXError = [LXError class];
        _resultCacheDuration = 86400;
    }
    return self;
}

- (AFHTTPRequestSerializer *)requestSerializer {
    if (!_requestSerializer) {
        _requestSerializer = [AFHTTPRequestSerializer serializer];
        _requestSerializer.timeoutInterval = _timeoutInterval;
    }
    return _requestSerializer;
}

- (AFHTTPResponseSerializer *)responseSerializer {
    if (!_responseSerializer) {
        _responseSerializer = [AFJSONResponseSerializer serializer];
    }
    return _responseSerializer;
}

- (instancetype)copyWithZone:(NSZone *)zone {
    LXNetworkingConfig *configuration = [[LXNetworkingConfig alloc] init];
    configuration.resultCacheDuration = self.resultCacheDuration;
    configuration.requestCachePolicy = self.requestCachePolicy;
    configuration.baseURL = [self.baseURL copy];
    configuration.builtinHeaders = [self.builtinHeaders copy];
    configuration.builtinBodys = [self.builtinBodys copy];
    configuration.resposeHandle = [self.resposeHandle copy];
    configuration.requestSerializer = [self.requestSerializer copy];
    configuration.responseSerializer = [self.responseSerializer copy];
    configuration.responseSerializer.acceptableContentTypes = self.responseSerializer.acceptableContentTypes;
    configuration.LXError = self.LXError;
    return configuration;
}

@end
