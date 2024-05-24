/* 
说明： 
功能：封装网络功能
作者：QHXRPG
日期：2024/5/23
*/
#pragma once
#include"disposable.h"
#include<map>
#include<errno.h>
#include<fcntl.h>  // 文件控制
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Unix标准头文件
#include <arpa/inet.h> //IP地址转换函数
#include <netinet/in.h>  // IPv4地址结构定义
#include <sys/types.h>  // Unix 系统数据类型定义
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "connect_obj.h"


#define INVALID_SOCKET -1

#define _sock_init( )
#define _sock_nonblock( sockfd ) { int flags = fcntl(sockfd, F_GETFL, 0); fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); }
#define _sock_exit( )
#define _sock_err( )	errno
#define _sock_close( sockfd ) ::close( sockfd )
#define _sock_is_blocked()	(errno == EAGAIN || errno == 0)

class ConnectObj;

class Network : public IDisposable
{
protected:
    static void SetSocketOpt(int socket);
    int CreateSocket();

    // 存储连接的 socket 或者监听的 socket
    int _masterSocket
    {
        INVALID_SOCKET
    };

    // 对于监听类来说，有多个 ConnectObj
    // 对于连接类来说，只有一个 ConnectObj
    // <socket, ConnectObj>
    std::map<int, ConnectObj*> _connects;

    // 可读、可写、异常的文件描述符集合
    fd_set readfds, writefds, exceptfds;

public:
    void Dispose() override;
    bool Select();
};






