# LXNetworking
> 基于AFNetworking的v3.2.1进行网络请求，基于PINCache进行网络数据缓存，支持清除指定url缓存、url及参数组合缓存，通过AFNetworkActivityLogger进行网络log打印。该代码使用灵活的请求方式，不包含任何业务代码，上层支持集中式、分布式网络接口管理方式，在请求前可以对请求进行配置，也支持对网络请求后返回的数据进行统一处理。

### 一、实现功能

#### **1、多环境切换功能：**

支持不同环境，快速切换对应环境的地址，也支持特殊的接口使用特殊环境的地址：

```objective-c
// 业务层代码
self.requestConvertManager = [LXRequestConvertManager sharedInstance];
// 不同环境设置不同的baseURL
self.requestConvertManager.configuration.baseURL = baseURL;

// 特殊的接口需要设置特殊的baseURL
[self.requestConvertManager requestMethod:LXRequestMethodPost
                                    URLString:SHOWAPI_LAUGHTER
                                   parameters:@{@"page" : @(pageIndex), @"maxResult" : @(pageSize)}
                         configurationHandler:^(LXRequestManagerConfig * _Nullable configuration) {
                             configuration.baseURL = otherBaseURL;
                         } cache:^(id  _Nullable responseObject) {
                             NSLog(@"缓存数据");
                             cache(responseObject);
                         } success:^(NSURLSessionTask * _Nullable dataTask, id  _Nullable responseObject) {
                             success(dataTask, responseObject);
                         } failure:^(NSURLSessionTask * _Nullable dataTask, LXError * _Nullable error) {
                             failure(dataTask, error);
                         }];
```

#### **2、处理多个接口中的通用配置：**

通用配置如 **请求的头部通用配置**、 **请求体的通用配置**、**接口的网络超时时长** 等等。

```objective-c
// 业务层代码
self.requestConvertManager = [LXRequestConvertManager sharedInstance];

//通过configuration来设置请求头
NSMutableDictionary *builtinHeaders = [NSMutableDictionary dictionary];
builtinHeaders[@"showapi_appid"] = SHOWAPI_APPID;
builtinHeaders[@"showapi_sign"] = SHOWAPI_SIGN;
self.requestConvertManager.configuration.builtinHeaders = builtinHeaders;

//通过configuration来设置通用的请求体
NSMutableDictionary *builtinBodys = [NSMutableDictionary dictionary];
builtinBodys[@"showapi_appid"] = SHOWAPI_APPID;
builtinBodys[@"showapi_sign"] = SHOWAPI_SIGN;
self.requestConvertManager.configuration.builtinBodys = builtinBodys;
```

#### **3、处理不同接口，存在的差异性：**

这里的差异性比如 **接口的网络超时时长**、**数据缓存时长**、**数据缓存协议** 等等，其实这里的几个地方，都有默认值，支持差异化请求。

```objective-c
// 业务层代码
self.requestConvertManager = [LXRequestConvertManager sharedInstance];

// 特殊的接口需要设置特殊的baseURL
[self.requestConvertManager requestMethod:LXRequestMethodPost
                                    URLString:SHOWAPI_LAUGHTER
                                   parameters:@{@"page" : @(pageIndex), @"maxResult" : @(pageSize)}
                         configurationHandler:^(LXRequestManagerConfig * _Nullable configuration) {
                           //设置缓存时长为100000秒
							configuration.resultCacheDuration = 100000;    
                           //优先取缓存数据，不在请求网络数据
                            configuration.requestCachePolicy = LXRequestReturnLoadToCache;    
                         } cache:^(id  _Nullable responseObject) {
                             NSLog(@"缓存数据");
                             cache(responseObject);
                         } success:^(NSURLSessionTask * _Nullable dataTask, id  _Nullable responseObject) {
                             success(dataTask, responseObject);
                         } failure:^(NSURLSessionTask * _Nullable dataTask, LXError * _Nullable error) {
                             failure(dataTask, error);
                         }];
```

#### **4、对网络请求返回的数据做一层拦截：**

这里举例说明一下，就是一些app中，登录态只是持续一段时间的，但是一些接口必须要登录态有效才能访问的，这个时候一般姿势可能是，对这些需要判断登录态的接口的返回数据进行筛选判断，然后再做下一步处理，随着这样的接口满满的变多，就导致搞不清到底这一套判断写在哪里了。可能有遇到过这种情况的老司机选择了在自己封装的网络请求 [AFNetworking](https://github.com/AFNetworking/AFNetworking)  那段代码返回的时候统一的加上这层逻辑，这的确是一种省时省力的方式，但是问题就在于老司机做的这层 组件 也被业务代码给玷污了。😂

我的思路其实也是如此，这层拦截的代码也是加在网络请求 [AFNetworking](https://github.com/AFNetworking/AFNetworking)  那段代码返回的地方，但是我使用了block，把原始数据回调给业务层本身，也就意味了业务层自己处理自己的拦截。

```objective-c
// 业务层代码
self.requestConvertManager = [LXRequestConvertManager sharedInstance];

//通过configuration来统一处理输出的数据，比如对token失效处理、对需要重新登录拦截
self.requestConvertManager.configuration.resposeHandle = ^id (NSURLSessionTask *dataTask, id responseObject) {
  	responseObject = [responseObject doSomething];
	return responseObject;
};


//LXNetworkingManager.m
self.requestManager = [AFHTTPSessionManager manager];
[self.requestManager dataTaskWithRequest:request
completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
    __strong typeof(self) strong_self = weak_self;
    if (error) {
        
    }
    else {
      	//如果业务层需要拦截，则先将返回的数据抛给业务层先处理
        if (configuration.resposeHandle) {
            responseObject = configuration.resposeHandle(dataTask, responseObject);
        }
        success(dataTask, responseObject);
    }
}];
```

#### **5、网络数据缓存策略**：

缓存这块使用的是 [PINCache](https://github.com/pinterest/PINCache) ，策略分了五种方式。
同时支持自定义缓存时间
```//设置缓存时长为100000秒
configuration.resultCacheDuration = 100000;```

 >缓存策略机制
 
 - LXRequestReturnCacheDontLoad: 如果缓存有效则直接返回缓存，缓存失效则返回nil，不再load。（场景：几乎没有任何变化的接口，实效性低）
 - LXRequestReturnCacheAndLoadToCache: 如果缓存有效则直接返回缓存，并且load且缓存数据。缓存失效则load返回，且缓存数据。（场景：接口实效性不高，但需要有一定实效性，比如商品详情接口）
 - LXRequestReturnCacheOrLoadToCache: 如果缓存有效则直接返回缓存。缓存失效则load返回，且缓存数据。（场景：接口实效性不高，但需要有一定实效性，比如商品详情接口）
 - LXRequestReturnLoadToCache: 直接load并返回数据，且缓存数据，如果load失败则读取缓存数据。（场景：接口需要一定的实效性，但同时要有数据支持，比如项目的首页接口）
 - LXRequestReturnLoadDontCache: 直接load并返回数据，不缓存数据，如果load失败则直接抛出Error。（场景：接口一定是实时的，并且保证返回的数据真实、可靠、安全，而非本地缓存数据，比如支付接口）
 



#### **6、log日志的完善机制：**

这块使用的是 AFNetworkActivityLogger 这个类来实现，它也是可以根据开发者选择的log等级来打印对应级别的log，支持打印请求的 `HTTPMethod` 、`URL` 、`absluteString` 、网络请求响应的`statusCode` 、本次请求耗时等等信息，它是利用  [AFNetworking](https://github.com/AFNetworking/AFNetworking)  请求发出的通知来实现的，具体看源码。

#### **7、error层的自定义统一管理：**

组件中已经包含一个比较基础的 LXError 类，业务层继承这个然后重载如下的两个类方法即可：

```objective-c
+ (LXError *)lxErrorNetNotReachable;
+ (LXError *)lxErrorHttpError:(NSError *)error;
```

#### **8、网络状态的判断：**

这个没有好说的，还是 [AFNetworking](https://github.com/AFNetworking/AFNetworking) 那一套：

```objective-c
//LXNetworkingManager.m
[[AFNetworkReachabilityManager sharedManager] setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
	NSLog(@"Reachability: %@", AFStringFromNetworkReachabilityStatus(status));
	self.networkStatus = status;
}];
```

#### 9、取消网络请求：

在调用组件中的网络接口时，本身是返回请求的对象 `NSURLSessionTask` 的，如果需要取消，只需要拿到这个对象，执行 cannel 即可，当然，所有的请求都在 [AFNetworking](https://github.com/AFNetworking/AFNetworking) 排好队的，组件也提供接口来取消队列中的所有请求：

```objective-c
//LXNetworkingManager.m
- (void)cancelAllRequest {
    [self.requestManager invalidateSessionCancelingTasks:YES];
}
```


## 如何使用
```
pod "LXNetworking"
```
