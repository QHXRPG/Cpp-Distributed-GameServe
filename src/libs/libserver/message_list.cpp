#include "message_list.h"
#include "iostream"
#include "packet.h"

// 消息注册
void MessageList::RegisterFunction(int msgId, HandleFuntion function)
{
    std::lock_guard<std::mutex> guard(_msgMutex);
    _callbackHandle[msgId] = function;
}


// 判断该协议号是否是自己关心的
bool MessageList::IsRegisteredMsg(int msgId)
{
    std::lock_guard<std::mutex> guard(_msgMutex);

    // 是自己感兴趣的消息，返回ture，否则false
    auto iter = _callbackHandle.find(msgId);
    if(iter == _callbackHandle.end())
        return false;
    else
        return true;
}


// 消息处理
void MessageList::ProcessPacket()
{
    // 创建一个临时变量存储消息队列，缩短锁临界区
    std::list<Packet*> _tmpList;
    _msgMutex.lock();
    std::copy(_msgList.begin(), _msgList.end(), std::back_inserter(_tmpList));
    _msgList.clear();
    _msgMutex.unlock();

    for(auto packet : _tmpList)
    {
        const auto handleIter = _callbackHandle.find(packet->GetMsgId());
        if (handleIter == _callbackHandle.end())
        {
            std::cout << "该消息没有对应的方法实现. msg id:" << packet->GetMsgId() << std::endl;
        }
        else
        {
            handleIter->second(packet);
        }
    }
    _tmpList.clear();
}


// 将Thread 传给 Actor 的包加进 _msgList, 
// 这里的包已经过过滤处理，都是该Actor感兴趣的消息
void MessageList::AddPacket(Packet *pPacket)
{
    std::lock_guard<std::mutex> guard(_msgMutex);
    _msgList.push_back(pPacket);
}
