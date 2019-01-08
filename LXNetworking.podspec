#
#  Be sure to run `pod spec lint LXCategories.podspec' to ensure this is a
#  valid spec and to remove all comments including this before submitting the spec.
#
#  To learn more about Podspec attributes see http://docs.cocoapods.org/specification.html
#  To see working Podspecs in the CocoaPods repo see https://github.com/CocoaPods/Specs/
#

Pod::Spec.new do |s|

    s.name         = "LXNetworking"
    s.version      = "1.0.2"
    s.ios.deployment_target = '10.0'
    s.summary      = "好用网络请求库，基于AFNetworking v3.2.1。提供常用请求，上传，下载。缓存，设置缓存策略，缓存时间等功能"
    s.homepage     = "https://github.com/xinge1/LXNetworking"
    s.social_media_url = 'https://github.com/xinge1/LXNetworking'
    s.license      = "MIT"
    # s.license    = { :type => "MIT", :file => "FILE_LICENSE" }
    s.author       = { "xinge1" => "3093496743@qq.com" }
    s.source       = { :git => 'https://github.com/xinge1/LXNetworking.git', :tag => s.version}
    s.requires_arc = true
    s.source_files = 'LXNetworking/*.{h,m}'
    s.public_header_files = 'LXNetworking/*.{h}'
    s.dependency 'AFNetworking'
    s.dependency 'PINCache'

end
