//
//  AFNetworkActivityLogger.h
//  LXNetworking
//
//  Created by liuxin on 2019/1/7.
//  Copyright © 2019年 liuxin. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, AFHTTPRequestLoggerLevel) {
    AFLoggerLevelOff,
    AFLoggerLevelDebug,
    AFLoggerLevelInfo,
    AFLoggerLevelWarn,
    AFLoggerLevelError,
    AFLoggerLevelFatal = AFLoggerLevelOff,
};

NS_ASSUME_NONNULL_BEGIN

/**
 `AFNetworkActivityLogger` logs requests and responses made by AFNetworking, with an adjustable level of detail.
 
 Applications should enable the shared instance of `AFNetworkActivityLogger` in `AppDelegate -application:didFinishLaunchingWithOptions:`:
 
 [[AFNetworkActivityLogger sharedLogger] startLogging];
 
 `AFNetworkActivityLogger` listens for `AFNetworkingOperationDidStartNotification` and `AFNetworkingOperationDidFinishNotification` notifications, which are posted by AFNetworking as request operations are started and finish. For further customization of logging output, users are encouraged to implement desired functionality by listening for these notifications.
 */
@interface AFNetworkActivityLogger : NSObject

/**
 The level of logging detail. See "Logging Levels" for possible values. `AFLoggerLevelInfo` by default.
 */
@property (nonatomic, assign) AFHTTPRequestLoggerLevel level;

/**
 Omit requests which match the specified predicate, if provided. `nil` by default.
 
 @discussion Each notification has an associated `NSURLRequest`. To filter out request and response logging, such as all network activity made to a particular domain, this predicate can be set to match against the appropriate URL string pattern.
 */
@property (nonatomic, strong) NSPredicate *filterPredicate;

/**
 Returns the shared logger instance.
 */
+ (instancetype)sharedLogger;

/**
 Start logging requests and responses.
 */
- (void)startLogging;

/**
 Stop logging requests and responses.
 */
- (void)stopLogging;


@end

NS_ASSUME_NONNULL_END
