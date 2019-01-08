//
//  LXError.m
//  LXNetworking
//
//  Created by liuxin on 2019/1/7.
//  Copyright © 2019年 liuxin. All rights reserved.
//

#import "LXError.h"

@implementation LXError

static inline NSDictionary* errorMessageMap() {
    return @{
             @"999999" : @"当前暂无网络",
             };
}


+ (LXError *)lxErrorNetNotReachable {
    LXError *lxError = [LXError new];
    lxError.errorCode = @"999999";
    NSString *errorMessage = errorMessageMap()[lxError.errorCode];
    if (errorMessage) {
        lxError.errorMessage = errorMessage;
    }
    else {
        lxError.errorMessage = @"当前暂无网络";
    }
    lxError.errorMessageDescription = @"AFNetwork监测当前暂无网络";
    return lxError;
}

+ (LXError *)lxErrorHttpError:(NSError *)error {
    LXError *lxError = [LXError new];
    lxError.errorCode = [NSString stringWithFormat:@"%d", (int)error.code];
    lxError.errorMessage = error.localizedDescription;
    lxError.errorMessageDescription = error.localizedDescription;
    return lxError;
}

@end
