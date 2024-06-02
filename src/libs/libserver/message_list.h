#pragma once
#include <mutex>
#include <map>
#include <list>
#include <functional>

class Packet;

// ָ��һ�������� һ�� Packet* ������ void���Ļص����� 
typedef std::function<void(Packet*)> HandleFunction;

class MessageList
{
public:
    // ע�᷽��
    void RegisterFunction(int msgId, HandleFunction function);

    // �Ƿ��Ǹ�Actor����Ȥ��Ϣ
    bool IsFollowMsgId(int msgId);

    // ���� Packet
    void ProcessPacket();

    // ��� Packet
    void AddPacket(Packet* pPacket);

protected:
    std::mutex _msgMutex;
    std::list<Packet*> _msgList;
    std::map<int, HandleFunction> _callbackHandle;
};
