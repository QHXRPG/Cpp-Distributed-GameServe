#include<iostream>
#include "network_listen.h"
#include "connect_obj.h"


// 监听
bool NetworkListen::Listen(std::string ip, int port)
{
    // 把用于监听的socket存放在_masterSocket中
    _masterSocket = CreateSocket();

    if(_masterSocket == -1)
        return false;
    
    // 绑定监听端口
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);
    if(::bind(_masterSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        std::cout << " ip、端口绑定失败:"<< _sock_err() << std::endl;
        return false;
    }

    // 设置等待队列的长度
    int backlog = 10;
    char* ptr;
    if((ptr = getenv("LISTENQ")) != nullptr)
        backlog = atoi(ptr);

    // 开始监听
    if(::listen(_masterSocket, backlog) < 0) // 在此阻塞
    {
        std::cout << "监听启动失败:" << _sock_err() <<std::endl;
        return false;
    }
    return true;
}

// 接收
int NetworkListen::Accept()
{
    sockaddr socketClient;
    socklen_t socketLength = sizeof(socketClient);
    int rs = 0;

    // 在一个死循环中不断接收连接的socket，直到等待队列中的socket都被接收完
    while(true)
    {
        int socket = ::accept(_masterSocket, &socketClient, &socketLength);

        // 等待队列中的socket都被接收完,返回接收到socket的数量
        if(socket == -1)
            return rs;
        
        // 为新建的socket设置参数
        SetSocketOpt(socket);

        // 将socket以及其连接对象存入_connects中
        ConnectObj* connect = new ConnectObj(this, socket);
        _connects.insert(std::make_pair(socket, connect));
        rs ++;
    }
    return rs;
}


// 函数每帧都会调用，称为帧函数
bool NetworkListen::Update()
{
    //调用Select()对已连接的fd进行持续检测
    bool check = Select();

    // 如果监听socket可读，表示有连接
    if(FD_ISSET(_masterSocket, &readfds))
    {
        Accept();
    }
    return check;
}
