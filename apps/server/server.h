/* 
说明： 
功能：网络服务类，提供数据收发的功能
作者：QHXRPG
日期：2024/5/23
*/

#pragma once
#include"network/network_listen.h"

class Server : public NetworkListen
{
protected:
    void HandlerOne(ConnectObj* connect);
    int Accept() override;
    
    int _acceptCount{ 0 };    
	int _sendMsgCount{ 0 };
	int _recvMsgCount{ 0 };
	bool _isShow{ true };

public:
    bool DataHandler();
};

