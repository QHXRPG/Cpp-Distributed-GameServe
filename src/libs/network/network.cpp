#include "network.h"
#include "connect_obj.h"

#include <iostream>

void Network::Dispose()
{
	auto iter = _connects.begin();
	while (iter != _connects.end())
	{
		iter->second->Dispose();
		delete iter->second;
		++iter;
	}
	_connects.clear();

	//std::cout << "network dispose. close socket:" << _socket << std::endl;
	_sock_close(_masterSocket);
	_masterSocket = -1;
}

#ifndef WIN32
#define SetsockOptType void *
#else
#define SetsockOptType const char *
#endif

void Network::SetSocketOpt(SOCKET socket)
{
	// 1.�˿ڹرպ�������������
	bool isReuseaddr = true;
	setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (SetsockOptType)&isReuseaddr, sizeof(isReuseaddr));

	// 2.���͡�����timeout
	int netTimeout = 3000; // 1000 = 1��
	setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (SetsockOptType)&netTimeout, sizeof(netTimeout));
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (SetsockOptType)&netTimeout, sizeof(netTimeout));

#ifndef WIN32

	int keepAlive = 1;
	socklen_t optlen = sizeof(keepAlive);

	int keepIdle = 60 * 2;	// ��socket û�н����� ��� ��ʼ��������
	int keepInterval = 10;	// ��η�������֮��ļ��
	int keepCount = 5;		// ��������

	setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (SetsockOptType)&keepAlive, optlen);
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

#endif

	// 3.������
	_sock_nonblock(socket);
}

SOCKET Network::CreateSocket()
{
	_sock_init();
	SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket == INVALID_SOCKET)
	{
		std::cout << "::socket failed. err:" << _sock_err() << std::endl;
		return socket;
	}

	SetSocketOpt(socket);
	return socket;
}

bool Network::Select()
{
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	FD_SET(_masterSocket, &readfds);
	FD_SET(_masterSocket, &writefds);
	FD_SET(_masterSocket, &exceptfds);

	SOCKET fdmax = _masterSocket;
	;
	for (auto iter = _connects.begin(); iter != _connects.end(); ++iter)
	{
		if (iter->first > fdmax)
			fdmax = iter->first;

		FD_SET(iter->first, &readfds);
		FD_SET(iter->first, &exceptfds);

		if (iter->second->HasSendData()) {
			FD_SET(iter->first, &writefds);
		}
		else
		{
			if (_masterSocket == iter->first)
				FD_CLR(_masterSocket, &writefds);
		}
	}

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 50 * 1000;
	int nfds = ::select(fdmax + 1, &readfds, &writefds, &exceptfds, &timeout);
	if (nfds <= 0)
		return true;

	auto iter = _connects.begin();
	while (iter != _connects.end())
	{
		if (FD_ISSET(iter->first, &exceptfds))
		{
			std::cout << "socket except!! socket:" << iter->first << std::endl;

			iter->second->Dispose();
			delete iter->second;
			iter = _connects.erase(iter);
			continue;
		}

		if (FD_ISSET(iter->first, &readfds))
		{
			if (!iter->second->Recv())
			{
				iter->second->Dispose();
				delete iter->second;
				iter = _connects.erase(iter);
				continue;
			}
		}

		if (FD_ISSET(iter->first, &writefds))
		{
			if (!iter->second->Send())
			{
				iter->second->Dispose();
				delete iter->second;
				iter = _connects.erase(iter);
				continue;
			}
		}

		++iter;
	}

	return true;
}
