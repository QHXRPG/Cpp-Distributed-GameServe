#pragma once
#include <mutex>
#include <map>
#include <list>
#include <functional>

class Packet;

typedef std::function<void(Packet*)> HandleFuntion;

// ��Ϣ������, ÿ��Actor����һ����Ϣ����
class MessageList
{
protected:
    std::mutex _msgMutex;
    std::list<Packet*> _msgList; // ��Ϣ����
    std::map<int, HandleFuntion> _callbackHandle;  // ������� <msgId, Funtion>

public:
    // ��Ϣע��
    void RegisterFunction(int msgId, HandleFuntion function);

    // �жϸ�Э����Ƿ����Լ����ĵ�
    bool IsRegisteredMsg(int msgId);

    // ��Ϣ����
    void ProcessPacket();

    // ����ǰ Actor �������packet��Ϣ���� ���뵽��Ϣ������
    void AddPacket(Packet* pPacket);
};