#include <iostream>
#include "connect_obj.h"
#include "network_connector.h"
#include "packet.h"

ConnectObj *NetworkConnector::GetConnectObj()
{
    auto iter = _connects.find(_masterSocket);
	if (iter == _connects.end())
	{
		std::cout << "找不到任何连接" << std::endl;
		return nullptr;
	}
	return iter->second;
}

bool NetworkConnector::Connect(std::string ip, int port)
{
    _ip = ip;
    _port = port;

    _masterSocket = CreateSocket();
    if(_masterSocket == -1)
        return false;
    
    // 连接指定ip和端口的服务器
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    ::inet_pton(AF_INET, _ip.c_str(), &addr.sin_addr.s_addr);
    int flag = ::connect(_masterSocket, (sockaddr*)&addr, sizeof(sockaddr));

    // 连接返回0表示连接成功
    if(flag == 0)
    {
        ConnectObj* connect = new ConnectObj(this, _masterSocket);
        _connects.insert(std::make_pair(_masterSocket, connect));
    }
    return true;
}

bool NetworkConnector::Update()
{
    const bool check = Select();

    if(! IsConnected()) // 如果未创建连接
    {
        if(FD_ISSET(_masterSocket, &exceptfds)) //判断这个socket是否在异常的集合中
        {
            std::cout << "发现异常socket:" << _masterSocket<<" 请重新连接"<<std::endl;

            // 删除该socket并重新连接
            Dispose();
            Connect(_ip, _port);
            return check;
        }

        // 监听类听到新连接触发读操作、连接类连接成功触发写操作
        if(FD_ISSET(_masterSocket, &readfds) || FD_ISSET(_masterSocket, &writefds))
        {
            int socketState = -1;
            socklen_t optlen = sizeof(socketState);

            // 拿到当前socket的状态
            const int result = ::getsockopt(_masterSocket, SOL_SOCKET,
                                            SO_ERROR, (char*)(&socketState), &optlen);
            
            
            if(result == 0 && socketState == 0) // 如果socket状态正常
            {
                // 生成一个 connnectObj 对象用于数据收发
                ConnectObj* connect = new ConnectObj(this, _masterSocket);
                _connects.insert(std::make_pair(_masterSocket, connect));
            }
            else  // 如果socket状态异常
            {
                std::cout << "连接失败. socket:" << _masterSocket << 
                                                    " 请重新链接." << std::endl;
                Dispose();
                Connect(_ip, _port);
            }
        }
    }
}

// 接收缓存区是否有数据或者数据是否完整可读
bool NetworkConnector::HasRecvData()
{
	int size = _connects.size();
	if (size <= 0)
		return false;

	if (size != 1)
	{
		std::cout << "Error. NetworkConnector has two connect!!!!" << std::endl;
		return false;
	}

	ConnectObj* pConnect = GetConnectObj();
	if (pConnect == nullptr)
		return false;

	return pConnect->HasRecvData();
}

// 拿到从对端传过来的报文并转成 Packet
Packet *NetworkConnector::GetRecvPacket()
{
	ConnectObj* pConnect = GetConnectObj();
	if (pConnect == nullptr)
		return nullptr;

	return pConnect->GetRecvPacket();
}

// 发送 Packet 到对端
void NetworkConnector::SendPacket(Packet *pPacket)
{ 
    ConnectObj* pConnect = GetConnectObj();
	if (pConnect == nullptr)
		return;

	pConnect->SendPacket(pPacket);
}

// 是否创建了连接,若该客户端创建了连接，那么_connects中存放服务端的socket
bool NetworkConnector::IsConnected() const
{
    return _connects.size() > 0;
}
