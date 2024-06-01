#pragma once
#include <mutex>
#include <map>
#include <list>
#include <functional>

class Packet;

typedef std::function<void(Packet*)> HandleFuntion;

// 消息队列类, 每个Actor都有一个消息队列
class MessageList
{
protected:
    std::mutex _msgMutex;
    std::list<Packet*> _msgList; // 消息队列
    std::map<int, HandleFuntion> _callbackHandle;  // 任务队列 <msgId, Funtion>

public:
    // 消息注册
    void RegisterFunction(int msgId, HandleFuntion function);

    // 判断该协议号是否是自己关心的
    bool IsRegisteredMsg(int msgId);

    // 消息处理
    void ProcessPacket();

    // 若当前 Actor 关心这个packet消息包， 加入到消息队列中
    void AddPacket(Packet* pPacket);
};