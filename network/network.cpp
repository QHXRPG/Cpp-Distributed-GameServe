#include"network.h"
#include "connect_obj.h"
#include <iostream>

// 为新连接上来的socket设置参数
// socket复用、收发超时时间、TCP保活机制、非阻塞
void Network::SetSocketOpt(int socket)
{
    // 1. 端口关闭后马上重新使用
    bool isReuseaddr = true;
	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (void *)&isReuseaddr, sizeof(isReuseaddr));

    // 2. 设置发送、接收的超时时间
    int netTimeout = 3000;      // 3秒
	setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (void *)&netTimeout, sizeof(netTimeout));
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (void *)&netTimeout, sizeof(netTimeout));

    int keepAlive = 1;
	socklen_t optlen = sizeof(keepAlive);

    // 3. 设置TCP保活机制
	int keepIdle = 60 * 2;	// 在socket 没有交互后 多久 开始发送侦测包
	int keepInterval = 10;	// 多次发送侦测包之间的间隔
	int keepCount = 5;		// 侦测包个数

	setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, optlen);
	if (getsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, &optlen) < 0)
	{
		std::cout << "getsockopt SO_KEEPALIVE failed." << std::endl;
	}

	setsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, optlen);
	if (getsockopt(socket, SOL_TCP, TCP_KEEPIDLE, &keepIdle, &optlen) < 0)
	{
		std::cout << "getsockopt TCP_KEEPIDLE failed." << std::endl;
	}

	setsockopt(socket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, optlen);
	setsockopt(socket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, optlen);

    // 4. 非阻塞 即使没有数据可读或可写，操作也会立即返回，而不会等待
    _sock_nonblock(socket);
}

int Network::CreateSocket()
{
    return 0;
}

// Network派生类的析构函数
void Network::Dispose()
{
    for(auto iter = _connects.begin(); iter!=_connects.end(); iter++)
    {
        iter->second->Dispose();
        delete iter->second;
    }

    // 清空_connects，并关闭监听/连接socket
    _connects.clear();
    _sock_close(_masterSocket);
    _masterSocket = -1;
}

bool Network::Select()
{
    // 初始化所有描述符集合, 全部置零
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    // 将用于监听/连接的 _masterSocket 存入三个集合中
    FD_SET(_masterSocket, &readfds);
    FD_SET(_masterSocket, &writefds);
    FD_SET(_masterSocket, &exceptfds);

    // 保存最大的 socket
    int fdmax = _masterSocket; //初始化
    for(auto iter = _connects.begin(); iter!=_connects.end(); iter++)
    {
        if(iter->first > fdmax)
            fdmax = iter->first;

        // 把当前遍历到的fd存入 可读、异常的fd集合
        FD_SET(iter->first, &readfds);
        FD_SET(iter->first, &exceptfds);

        // 如果遍历到的fd所对应的连接对象有数据要发送，将其fd存入可写集合
        if(iter->second->HasSendData())
        {
            FD_SET(iter->first, &writefds);
        }
        else
        {
            // _masterSocket不用于发送数据
            if(_masterSocket == iter->first)
            {
                FD_CLR(_masterSocket, &writefds);
            }
        }
    }

    /*                        *****所有fd收集完毕，开始启动Select*****                       */
    //设置50ms的时间间隔作为超时时间
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 50 * 1000;

    //将 fdmax 的值加一作为 select 函数的第一个参数,指定需要监视的fd范围
    int nfds = ::select(fdmax+1, &readfds, &writefds, &exceptfds, &timeout);

    // 没有fd，返回
    if(nfds <= 0)
    {
        return true;
    }

    // 若有fd， 遍历_connects
    auto iter = _connects.begin();
    while (iter != _connects.end())
    {
        // 处理异常fd
        if(FD_ISSET(iter->first, &exceptfds))
        {
            iter->second->Dispose();    // 调用该连接类的析构函数
            delete iter->second;
            iter = _connects.erase(iter); //删除_connects中对应的<fd, ConnectObj>
            continue;
        }

        // 处理可读fd
        if(FD_ISSET(iter->first, &readfds))
        {
            // 接收缓冲区没数据或已读完
            if(! iter->second->Recv())
            {
                iter->second->Dispose();    // 调用该连接类的析构函数
                delete iter->second;
                iter = _connects.erase(iter); //删除_connects中对应的<fd, ConnectObj>
                continue;
            }
        }

        // 处理可写fd
        if(FD_ISSET(iter->first, &writefds))
        {
            // 发送缓冲区没数据或已发完
            if(! iter->second->Send())
            {
                iter->second->Dispose();    // 调用该连接类的析构函数
                delete iter->second;
                iter = _connects.erase(iter); //删除_connects中对应的<fd, ConnectObj>
                continue;
            }
        }

        // 当前fd处理完毕
        iter ++;
    }
    return true;
}
