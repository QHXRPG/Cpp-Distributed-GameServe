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
- v1.0:
-   1. 添加了消息队列过滤器机制，根据指定的socket和packet给到对应的线程包裹类，可以取消有消息过滤需求的Actor取消息的互斥锁，将方法设置为noexpect防止空指针异常退出；
    2. 明确了HttpRequest类、Account类、Robot类的需求，Robot类负责发送用户请求、HttpRequest类负责处理用户请求并将用户请求或者服务器响应广播到框架中、Account类负责对用户的请求作出响应，并将请求交由HttpRequest类进行广播。（这里所说的广播就是将消息加入到消息队列）
- v1.1:
-   1. 引入状态机数据结构，让Actor在不同的状态下作出不同的反应，每个状态只需要管理好自己的帧函数以及退出事件。每次状态变化，状态机都会广播给框架，notify相应的程序作出反应。（每个状态创建一个继承自状态机的类，并让对应Actor将该状态类注册到状态机管理器的<状态,方法>map中）
- v1.2:
- v1.3: 
- v1.4: 
- v1.5: 
- v1.6:
- ....
