#pragma once
#include <mutex>
#include <map>
#include <list>
#include <functional>

class Packet;

// 指定一个（接受 一个 Packet* 并返回 void）的回调函数 
typedef std::function<void(Packet*)> HandleFunction;

class MessageList
{
public:
    // 注册方法
    void RegisterFunction(int msgId, HandleFunction function);

    // 是否是该Actor感兴趣消息
    bool IsFollowMsgId(int msgId);

    // 处理 Packet
    void ProcessPacket();

    // 添加 Packet
    void AddPacket(Packet* pPacket);

protected:
    std::mutex _msgMutex;
    std::list<Packet*> _msgList;
    std::map<int, HandleFunction> _callbackHandle;
};
