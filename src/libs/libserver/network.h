#pragma once

#include <map>
#include "common.h"
#include "entity.h"
#include "socket_object.h"
#include "cache_swap.h"
#include "message_system.h"
#include "network_interface.h"

#if ENGINE_PLATFORM != PLATFORM_WIN32
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#ifdef EPOLL
#include <sys/epoll.h>
#endif

#define _sock_init( )
#define _sock_nonblock( sockfd ) { int flags = fcntl(sockfd, F_GETFL, 0); fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); }
#define _sock_exit( )
#define _sock_err( )	errno
#define _sock_close( sockfd ) ::shutdown( sockfd, SHUT_RDWR ) 
#define _sock_is_blocked()	(errno == EAGAIN || errno == 0)

#define RemoveConnectObj(iter) \
    iter->second->ComponentBackToPool( ); \
    DeleteEvent(_epfd, iter->first); \
    iter = _connects.erase( iter ); 

#else

#define _sock_init( )	{ WSADATA wsaData; WSAStartup( MAKEWORD(2, 2), &wsaData ); }
#define _sock_nonblock( sockfd )	{ unsigned long param = 1; ioctlsocket(sockfd, FIONBIO, (unsigned long *)&param); }
#define _sock_exit( )	{ WSACleanup(); }
#define _sock_err( )	WSAGetLastError()
#define _sock_close( sockfd ) ::closesocket( sockfd )
#define _sock_is_blocked()	(WSAGetLastError() == WSAEWOULDBLOCK)

#define RemoveConnectObj(iter) \
    iter->second->ComponentBackToPool( ); \
    iter = _connects.erase( iter ); 

#endif

class ConnectObj;
class Packet;

class Network : public Entity<Network>, public INetwork
{
public:
    void BackToPool() override;
    SOCKET GetSocket() override { return _masterSocket; }
    void SendPacket(Packet*& pPacket) override;
    bool IsBroadcast() { return _isBroadcast; }

protected:
    static void SetSocketOpt(SOCKET socket);
    static SOCKET CreateSocket();
    void CreateConnectObj(SOCKET socket);
    void Clean();

#ifdef EPOLL
    void InitEpoll();
    void Epoll();
    void AddEvent(int epollfd, int fd, int flag);
    void ModifyEvent(int epollfd, int fd, int flag);
    void DeleteEvent(int epollfd, int fd);
#else

    void Select();
#endif

    void OnNetworkUpdate();

protected:
    SOCKET _masterSocket{ INVALID_SOCKET };
    std::map<SOCKET, ConnectObj*> _connects;

#ifdef EPOLL
#define MAX_CLIENT  5120
#define MAX_EVENT   5120
    struct epoll_event _events[MAX_EVENT];
    int _epfd;
    int _mainSocketEventIndex{ -1 };
#else
    fd_set readfds, writefds, exceptfds;
#endif

    // ����Э��
    std::mutex _sendMsgMutex;
    CacheSwap<Packet> _sendMsgList;

    // �յ���Э���Ƿ���Ҫ�㲥��ȫ�߳�
    bool _isBroadcast{ true };
};
