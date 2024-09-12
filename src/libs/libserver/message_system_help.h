#pragma once
#include "common.h"
#include "app_type_mgr.h"

class Packet;

class MessageSystemHelp
{
public:
    // ����һ���µİ�
    static Packet* CreatePacket(Proto::MsgId msgId, SOCKET socket);

    // �ַ���
    static void DispatchPacket(const Proto::MsgId msgId, const SOCKET socket);
    static void DispatchPacket(const Proto::MsgId msgId, const SOCKET socket, google::protobuf::Message& proto);

    // ���Ͱ�
    static void SendPacket(const Proto::MsgId msgId, const SOCKET socket, google::protobuf::Message& proto);
    static void SendPacket(const Proto::MsgId msgId, google::protobuf::Message& proto, APP_TYPE appType, int appId = 0);

protected:
    // �ַ�����ʵ��
    static void DispatchPacket(Packet* packet);

    // ���Ͱ���ʵ��
    static void SendPacket(Packet* packet, APP_TYPE appType, int appId = 0);
    static void SendPacket(Packet* pPacket);
};
