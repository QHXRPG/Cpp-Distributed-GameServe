/* 
说明： 
功能：客户端连接类
作者：QHXRPG
日期：2024/5/24
*/

#pragma once
#include "network.h"

class ConnectObj;
class Packet;

class NetworkConnector : public Network
{
private:
    std::string _ip;
    int _port;
    ConnectObj* GetConnectObj();

public:
    bool Connect(std::string ip, int port);     //连接指定ip和端口的服务器
    bool Update();                              //每帧调用 Select()函数对网络连接进行检查
    bool HasRecvData();                         // 是否收到数据
    Packet* GetRecvPacket();                    // 将接收到的数据转成 Packet
    void SendPacket(Packet* pPacket);           // 发送数据到对端
    bool IsConnected() const;
};