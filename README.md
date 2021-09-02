# Decoy

## 功能
- 实现 wifi adb 功能
- 实现 http 代理（一键配置抓包）

## 技术点
- 动态注册jni接口
- native层执行shell命令并获取返回
- native层以root身份执行命令
- 利用原生gethostbyname实现native层获取域名对应ip地址
- Decoy 名称来源于Rick and Morty S5E2

## TODO
- native层及java层 root检测
- 反调试
- 在native层实现网络api调用
- 接入jetpack compose制作ui

## in the fulture
- 整理接入 [vlmcsd](https://github.com/Wind4/vlmcsd)
- 接入ollvm
