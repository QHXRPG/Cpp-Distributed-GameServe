#include"network.h"

void Network::SetSocketOpt(int socket)
{
}

int Network::CreateSocket()
{
    return 0;
}

Network::Network()
{
}

Network::~Network()
{
}

void Network::Dispose()
{
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
}
