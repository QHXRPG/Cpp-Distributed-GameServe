#include "server.h"
#include "network/packet.h"
#include "network/connect_obj.h"
#include <iostream>

void Server::HandlerOne(ConnectObj *connect)
{
    
    while(connect->HasRecvData())  // 接收缓存区有数据要读
    {
        Packet* packet = connect->GetRecvPacket();  // 把接收缓存区中二进制数据转成 packet
        
        // 数据不全，退出循环，下次再取出来检查
        if(packet == nullptr)
            break;

        //回显业务 : 将报文转成字符串再发送回去
        std::string msg(packet->GetBuffer(), packet->GetDataLength());
        std::cout << "recv size:" << msg.length() << " msg:" << msg.c_str() << std::endl;
        connect->SendPacket(packet);
        
        _recvMsgCount ++;
        _sendMsgCount ++;
        _isShow = true;
    }
}


bool Server::DataHandler()
{
    for(auto iter = _connects.begin(); iter!=_connects.end(); iter++)
    {
        ConnectObj* connect = iter->second;
        HandlerOne(connect);
    }
    if(_isShow)
    {
        _isShow = false;
        std::cout << "接收连接数:" << _acceptCount << "\t接收报文数量:" <<
         _recvMsgCount << "\t发送报文数量:" << _sendMsgCount << std::endl;
    }
}


int Server::Accept()
{
    int nums = NetworkListen::Accept();
    _acceptCount += nums;
    _isShow = true;
    return nums;
}

