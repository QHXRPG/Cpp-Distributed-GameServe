#pragma once
#include "common.h"
#include "app_type_mgr.h"

class Packet;

class MessageSystemHelp
{
public:
    // 创建一个新的包
    static Packet* CreatePacket(Proto::MsgId msgId, SOCKET socket);

    // 分发包
    static void DispatchPacket(const Proto::MsgId msgId, const SOCKET socket);
    static void DispatchPacket(const Proto::MsgId msgId, const SOCKET socket, google::protobuf::Message& proto);

    // 发送包
    static void SendPacket(const Proto::MsgId msgId, const SOCKET socket, google::protobuf::Message& proto);
    static void SendPacket(const Proto::MsgId msgId, google::protobuf::Message& proto, APP_TYPE appType, int appId = 0);

protected:
    // 分发包的实现
    static void DispatchPacket(Packet* packet);

    // 发送包的实现
    static void SendPacket(Packet* packet, APP_TYPE appType, int appId = 0);
    static void SendPacket(Packet* pPacket);
};
