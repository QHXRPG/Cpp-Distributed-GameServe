# Cpp-Distributed-GameServe

## 项目简介
对之前大型多人在线游戏的服务器框架进行改进与升级。
![image](https://github.com/QHXRPG/Cpp-Distributed-GameServe/blob/main/%E4%B8%BB%E4%BB%8E%E6%9C%8D%E5%8A%A1.png)
### 环境
1. Centos 7;
2. gcc 9.1.0;
3. protobuf(cpp) 3.9.1;
4. protobuf(C#) 3.9.1;
5. PHP 5.4.16 (fpm-fcgi)
6. nginx 1.20.1
7. jsoncpp 1.9.1
## 改进计划
1. 将大型多人在线服务器框架从Windows移植到Linux；
2. 采用C++11重构整个服务器框架；
3. 使用主从Reactor多线程网络模型处理处理网络 I/O ；
4. 利用Actor模型降低代码耦合度，利用Actor间消息通信减少加锁操作；
5. 基于Actor模型，搭建组件编程的ECS框架；
6. 将Redis与MySQL做相关结合，减轻MySQL数据库的负载；
7. 利用Nginx处理账号的登录与验证；
8. 使用相关分布式技术实现分布式地图跳转等功能；
9. 实现用户断线、超时重连等功能。
![image](https://github.com/QHXRPG/Cpp-Distributed-GameServe/blob/main/%E6%B6%88%E6%81%AF%E4%BC%A0%E8%BE%93.png)
## 版本历史
- v1.0: 添加了消息队列过滤器机制，根据指定的socket和packet给到对应的线程包裹类，可以取消消息出队入队的互斥锁，将方法设置为noexpect防止空指针异常退出
- v1.1: 
- v1.2:
- v1.3: 
- v1.4: 
- v1.5: 
- v1.6:
- ....

## 如何运行
1. 安装依赖库和工具；
2. 编译项目源码；
3. 运行服务器程序。

感谢您的关注与支持！
